#include "RenderDamage.h"

namespace {

enum TPhase {
    EIdle,
    ERecording,
    EPainting,
    EFullPaintRecording
};

// Draw calls that belong to a Drawable are folded into one compact record.
// This small stream is only for operations outside a Drawable (background,
// overlays, etc.), which keeps dense maps from spending CPU diffing thousands
// of individual blits.
const int KCommandCapacity = 256;
const int KDrawableCapacity = 2048;
const int KDirtyCapacity = 32;
const int KMaximumDirtyRegions = 4;
const int KFullRedrawAreaPercent = 94;
const int KDirtyRegionFixedCostDivisor = 10;
const int KCommandResyncLookahead = 8;
const int KDrawableResyncLookahead = 12;
const int KAdaptiveEnterAreaPercent = 88;
const int KAdaptiveEnterScore = 6;
const int KAdaptiveScoreIncrease = 2;
const int KAdaptiveFullFrames = 2;

struct TCommand {
    unsigned int signature;
    Rect rect;
};

struct TDrawableRecord {
    Drawable* drawable;
    Rect rect;
    unsigned int signature;
    bool signatureKnown;
};

TPhase gPhase = EIdle;
TCommand gCommandsA[KCommandCapacity];
TCommand gCommandsB[KCommandCapacity];
TCommand* gPrevious = gCommandsA;
TCommand* gCurrent = gCommandsB;
int gPreviousCount = 0;
int gCurrentCount = 0;
bool gPreviousValid = false;
bool gOverflow = false;
bool gFullDependency = false;
bool gDamageValid = false;
bool gFullFrameRequested = false;
int gScrollX = 0;
int gScrollY = 0;
Rect gFrameRect;
Rect gPaintRect;
Rect gDirty[KDirtyCapacity];
int gDirtyCount = 0;
int gPaintRegion = -1;
int gAdaptiveScore = 0;
int gAdaptiveFullFrames = 0;
bool gAdaptiveFullPaintActive = false;
int gRecordSuppressionDepth = 0;
bool gFastFullPaint = false;
TDrawableRecord gDrawables[KDrawableCapacity];
int gDrawableCount = 0;
bool gDrawableOverflow = false;
Drawable* gActiveDrawable = nullptr;
Rect gActiveDrawableRect;
bool gActiveDrawableHasRect = false;
unsigned int gActiveDrawableSignature = 0;
bool gActiveDrawableSignatureKnown = true;
TDrawableRecord gPreviousDrawables[KDrawableCapacity];
int gPreviousDrawableCount = 0;
bool gPreviousDrawablesValid = false;

int Area(const Rect& aRect) {
    return aRect.width > 0 && aRect.height > 0 ? aRect.width * aRect.height : 0;
}

Rect UnionRect(const Rect& aFirst, const Rect& aSecond) {
    const int left = aFirst.x < aSecond.x ? aFirst.x : aSecond.x;
    const int top = aFirst.y < aSecond.y ? aFirst.y : aSecond.y;
    const int firstRight = aFirst.x + aFirst.width;
    const int secondRight = aSecond.x + aSecond.width;
    const int firstBottom = aFirst.y + aFirst.height;
    const int secondBottom = aSecond.y + aSecond.height;
    const int right = firstRight > secondRight ? firstRight : secondRight;
    const int bottom = firstBottom > secondBottom ? firstBottom : secondBottom;
    return Rect(left, top, right - left, bottom - top);
}

bool Touches(const Rect& aFirst, const Rect& aSecond) {
    const int firstRight = aFirst.x + aFirst.width;
    const int secondRight = aSecond.x + aSecond.width;
    const int firstBottom = aFirst.y + aFirst.height;
    const int secondBottom = aSecond.y + aSecond.height;
    return aFirst.x <= secondRight && aSecond.x <= firstRight &&
        aFirst.y <= secondBottom && aSecond.y <= firstBottom;
}

bool SameRect(const Rect& aFirst, const Rect& aSecond) {
    return aFirst.x == aSecond.x && aFirst.y == aSecond.y &&
        aFirst.width == aSecond.width && aFirst.height == aSecond.height;
}

Rect ShiftRect(const Rect& aRect, int aShiftX, int aShiftY) {
    return Rect(aRect.x + aShiftX, aRect.y + aShiftY, aRect.width, aRect.height);
}

void ForceFull() {
    gDirty[0] = gFrameRect;
    gDirtyCount = 1;
}

bool DamageComplete() {
    if (gDirtyCount <= 0)
        return false;
    if (gFullDependency) {
        ForceFull();
        return true;
    }
    return gDirtyCount == 1 && SameRect(gDirty[0], gFrameRect);
}

void RemoveDirty(int aIndex) {
    for (int index = aIndex + 1; index < gDirtyCount; ++index)
        gDirty[index - 1] = gDirty[index];
    --gDirtyCount;
}

void AddDirty(const Rect& aInput) {
    Rect rect = aInput;
    rect.Adjust(gFrameRect);
    if (rect.IsEmpty())
        return;
    if (gDirtyCount == 1 && SameRect(gDirty[0], gFrameRect))
        return;

    for (int index = 0; index < gDirtyCount; ++index) {
        if (Touches(gDirty[index], rect)) {
            gDirty[index] = UnionRect(gDirty[index], rect);
            for (int other = gDirtyCount - 1; other > index; --other) {
                if (Touches(gDirty[index], gDirty[other])) {
                    gDirty[index] = UnionRect(gDirty[index], gDirty[other]);
                    RemoveDirty(other);
                }
            }
            return;
        }
    }

    if (gDirtyCount >= KDirtyCapacity) {
        ForceFull();
        return;
    }
    gDirty[gDirtyCount++] = rect;
}

void TrackActiveDrawableRect(const Rect& aRect) {
    if (!gActiveDrawable)
        return;
    if (!gActiveDrawableHasRect) {
        gActiveDrawableRect = aRect;
        gActiveDrawableHasRect = true;
    } else {
        gActiveDrawableRect = UnionRect(gActiveDrawableRect, aRect);
    }
}

void TrackActiveDrawableSignature(unsigned int aSignature) {
    if (!gActiveDrawable)
        return;
    if (aSignature == 0) {
        gActiveDrawableSignatureKnown = false;
        return;
    }
    gActiveDrawableSignature = SymbianRenderDamage::HashValue(gActiveDrawableSignature, aSignature);
}

void AddScrollExposedDirty(int aShiftX, int aShiftY) {
    if (aShiftX == 0 && aShiftY == 0)
        return;
    const int frameWidth = gFrameRect.width;
    const int frameHeight = gFrameRect.height;
    if (frameWidth <= 0 || frameHeight <= 0)
        return;
    const int absX = aShiftX < 0 ? -aShiftX : aShiftX;
    const int absY = aShiftY < 0 ? -aShiftY : aShiftY;
    if (absX >= frameWidth || absY >= frameHeight) {
        ForceFull();
        return;
    }
    if (aShiftX > 0)
        AddDirty(Rect(0, 0, absX, frameHeight));
    else if (aShiftX < 0)
        AddDirty(Rect(frameWidth - absX, 0, absX, frameHeight));
    if (aShiftY > 0)
        AddDirty(Rect(0, 0, frameWidth, absY));
    else if (aShiftY < 0)
        AddDirty(Rect(0, frameHeight - absY, frameWidth, absY));
}

bool SameDrawableState(const TDrawableRecord& aPrevious,
                       const TDrawableRecord& aCurrent,
                       int aShiftX,
                       int aShiftY) {
    if (aPrevious.drawable != aCurrent.drawable ||
        !aPrevious.signatureKnown || !aCurrent.signatureKnown ||
        aPrevious.signature != aCurrent.signature)
        return false;
    // Two full-frame records are the scrolling background; everything else
    // counts as unchanged when it moved by exactly the map translation.
    if (SameRect(aPrevious.rect, gFrameRect) && SameRect(aCurrent.rect, gFrameRect))
        return true;
    return SameRect(ShiftRect(aPrevious.rect, aShiftX, aShiftY), aCurrent.rect);
}

// ponytail: this copy looks swappable like SaveCommandBaseline's pointer
// exchange, but the paint pass re-reads gDrawables after EndRecord, so the
// recorded frame must stay in place. Copy is load-bearing.
void SaveDrawableBaseline() {
    if (gDrawableOverflow) {
        gPreviousDrawableCount = 0;
        gPreviousDrawablesValid = false;
        return;
    }
    gPreviousDrawableCount = gDrawableCount;
    for (int index = 0; index < gDrawableCount; ++index)
        gPreviousDrawables[index] = gDrawables[index];
    gPreviousDrawablesValid = true;
}

void DiffDrawableRecords(int aShiftX, int aShiftY) {
    if (!gPreviousDrawablesValid || gDrawableOverflow) {
        ForceFull();
        return;
    }

    int previousIndex = 0;
    int currentIndex = 0;
    while (previousIndex < gPreviousDrawableCount && currentIndex < gDrawableCount) {
        const TDrawableRecord& previous = gPreviousDrawables[previousIndex];
        const TDrawableRecord& current = gDrawables[currentIndex];

        if (SameDrawableState(previous, current, aShiftX, aShiftY)) {
            ++previousIndex;
            ++currentIndex;
            continue;
        }
        if (previous.drawable == current.drawable) {
            AddDirty(ShiftRect(previous.rect, aShiftX, aShiftY));
            AddDirty(current.rect);
            ++previousIndex;
            ++currentIndex;
            if (DamageComplete())
                return;
            continue;
        }

        int previousMatch = -1;
        int currentMatch = -1;
        for (int offset = 1; offset <= KDrawableResyncLookahead; ++offset) {
            if (previousMatch < 0 && previousIndex + offset < gPreviousDrawableCount &&
                gPreviousDrawables[previousIndex + offset].drawable == current.drawable)
                previousMatch = offset;
            if (currentMatch < 0 && currentIndex + offset < gDrawableCount &&
                previous.drawable == gDrawables[currentIndex + offset].drawable)
                currentMatch = offset;
            if (previousMatch >= 0 || currentMatch >= 0)
                break;
        }

        if (previousMatch >= 0 && (currentMatch < 0 || previousMatch <= currentMatch)) {
            for (int offset = 0; offset < previousMatch; ++offset)
                AddDirty(ShiftRect(gPreviousDrawables[previousIndex + offset].rect, aShiftX, aShiftY));
            previousIndex += previousMatch;
            if (DamageComplete())
                return;
            continue;
        }
        if (currentMatch >= 0) {
            for (int offset = 0; offset < currentMatch; ++offset)
                AddDirty(gDrawables[currentIndex + offset].rect);
            currentIndex += currentMatch;
            if (DamageComplete())
                return;
            continue;
        }

        AddDirty(ShiftRect(previous.rect, aShiftX, aShiftY));
        AddDirty(current.rect);
        ++previousIndex;
        ++currentIndex;
        if (DamageComplete())
            return;
    }

    while (previousIndex < gPreviousDrawableCount)
        AddDirty(ShiftRect(gPreviousDrawables[previousIndex++].rect, aShiftX, aShiftY));
    while (currentIndex < gDrawableCount)
        AddDirty(gDrawables[currentIndex++].rect);
}

int DirtyArea() {
    int area = 0;
    for (int index = 0; index < gDirtyCount; ++index)
        area += Area(gDirty[index]);
    return area;
}

bool HasScroll() {
    return gScrollX != 0 || gScrollY != 0;
}

void OptimizeDirty() {
    if (gDirtyCount <= 0)
        return;

    while (gDirtyCount > KMaximumDirtyRegions) {
        int selectedFirst = 0;
        int selectedSecond = 1;
        int selectedIncrease = 0x7fffffff;
        for (int first = 0; first < gDirtyCount - 1; ++first) {
            for (int second = first + 1; second < gDirtyCount; ++second) {
                const Rect merged = UnionRect(gDirty[first], gDirty[second]);
                const int increase = Area(merged) - Area(gDirty[first]) - Area(gDirty[second]);
                if (increase < selectedIncrease) {
                    selectedFirst = first;
                    selectedSecond = second;
                    selectedIncrease = increase;
                }
            }
        }
        gDirty[selectedFirst] = UnionRect(gDirty[selectedFirst], gDirty[selectedSecond]);
        RemoveDirty(selectedSecond);
    }

    Rect combined = gDirty[0];
    for (int index = 1; index < gDirtyCount; ++index)
        combined = UnionRect(combined, gDirty[index]);

    const int frameArea = Area(gFrameRect);
    if (frameArea <= 0) {
        gDirtyCount = 0;
        return;
    }

    const int fixedCost = frameArea / KDirtyRegionFixedCostDivisor;
    const int separateCost = DirtyArea() + (gDirtyCount > 1 ? (gDirtyCount - 1) * fixedCost : 0);
    if (Area(combined) <= separateCost) {
        gDirty[0] = combined;
        gDirtyCount = 1;
    }

    if (DirtyArea() * 100 > frameArea * KFullRedrawAreaPercent)
        ForceFull();
}

bool SameCommand(const TCommand& aPrevious, const TCommand& aCurrent, int aShiftX, int aShiftY) {
    if (aPrevious.signature != aCurrent.signature)
        return false;
    if (SameRect(aPrevious.rect, gFrameRect) && SameRect(aCurrent.rect, gFrameRect))
        return true;
    return SameRect(ShiftRect(aPrevious.rect, aShiftX, aShiftY), aCurrent.rect);
}

void DiffCommands(int aShiftX, int aShiftY) {
    if (!gPreviousValid || gOverflow) {
        ForceFull();
        return;
    }

    int previousIndex = 0;
    int currentIndex = 0;
    while (previousIndex < gPreviousCount && currentIndex < gCurrentCount) {
        const TCommand& previous = gPrevious[previousIndex];
        const TCommand& current = gCurrent[currentIndex];

        if (SameCommand(previous, current, aShiftX, aShiftY)) {
            ++previousIndex;
            ++currentIndex;
            continue;
        }

        if (SameRect(ShiftRect(previous.rect, aShiftX, aShiftY), current.rect)) {
            AddDirty(current.rect);
            ++previousIndex;
            ++currentIndex;
            if (DamageComplete())
                return;
            continue;
        }

        int previousMatch = -1;
        int currentMatch = -1;
        for (int offset = 1; offset <= KCommandResyncLookahead; ++offset) {
            if (previousMatch < 0 && previousIndex + offset < gPreviousCount &&
                SameCommand(gPrevious[previousIndex + offset], current, aShiftX, aShiftY))
                previousMatch = offset;
            if (currentMatch < 0 && currentIndex + offset < gCurrentCount &&
                SameCommand(previous, gCurrent[currentIndex + offset], aShiftX, aShiftY))
                currentMatch = offset;
            if (previousMatch >= 0 || currentMatch >= 0)
                break;
        }

        if (previousMatch >= 0 && (currentMatch < 0 || previousMatch <= currentMatch)) {
            for (int offset = 0; offset < previousMatch; ++offset)
                AddDirty(ShiftRect(gPrevious[previousIndex + offset].rect, aShiftX, aShiftY));
            previousIndex += previousMatch;
            if (DamageComplete())
                return;
            continue;
        }
        if (currentMatch >= 0) {
            for (int offset = 0; offset < currentMatch; ++offset)
                AddDirty(gCurrent[currentIndex + offset].rect);
            currentIndex += currentMatch;
            if (DamageComplete())
                return;
            continue;
        }

        AddDirty(ShiftRect(previous.rect, aShiftX, aShiftY));
        AddDirty(current.rect);
        ++previousIndex;
        ++currentIndex;
        if (DamageComplete())
            return;
    }

    while (previousIndex < gPreviousCount)
        AddDirty(ShiftRect(gPrevious[previousIndex++].rect, aShiftX, aShiftY));
    while (currentIndex < gCurrentCount)
        AddDirty(gCurrent[currentIndex++].rect);
}

// Diff the recorded frame against the previous baseline. A pending scroll
// shifts the baseline instead of marking everything dirty.
void DiffFrame() {
    if (!gPreviousValid || !gPreviousDrawablesValid || gOverflow || gDrawableOverflow) {
        ForceFull();
        return;
    }
    AddScrollExposedDirty(gScrollX, gScrollY);
    if (!DamageComplete())
        DiffCommands(gScrollX, gScrollY);
    if (!DamageComplete())
        DiffDrawableRecords(gScrollX, gScrollY);
    OptimizeDirty();
}

void SaveCommandBaseline() {
    if (gOverflow) {
        gPreviousCount = 0;
        gPreviousValid = false;
        return;
    }
    TCommand* completed = gCurrent;
    gCurrent = gPrevious;
    gPrevious = completed;
    gPreviousCount = gCurrentCount;
    gPreviousValid = true;
}

void UpdateAdaptiveMode() {
    const int frameArea = Area(gFrameRect);
    if (frameArea <= 0) {
        gAdaptiveScore = 0;
        return;
    }

    const bool expensiveDamage =
        (gDirtyCount == 1 && SameRect(gDirty[0], gFrameRect)) ||
        DirtyArea() * 100 >= frameArea * KAdaptiveEnterAreaPercent;
    if (expensiveDamage) {
        gAdaptiveScore += KAdaptiveScoreIncrease;
        if (gAdaptiveScore > KAdaptiveEnterScore)
            gAdaptiveScore = KAdaptiveEnterScore;
    } else if (gAdaptiveScore > 0) {
        --gAdaptiveScore;
    }

    if (gAdaptiveScore >= KAdaptiveEnterScore) {
        gAdaptiveFullFrames = KAdaptiveFullFrames;
        gAdaptiveScore = KAdaptiveEnterScore / 2;
    }
}

}

namespace SymbianRenderDamage {

const Rect* gPaintClipRect = nullptr;

// Common frame-start reset. aClearScroll is false for BeginRecord (pending
// scroll survives into the diff) and for BeginFastFullPaint (EndFastFullPaint
// consumes it); BeginFullPaintRecord starts from a clean surface.
static void ResetFrameState(TPhase aPhase, int aWidth, int aHeight, bool aClearScroll) {
    gPhase = aPhase;
    gCurrentCount = 0;
    gOverflow = false;
    gFullDependency = false;
    gDamageValid = false;
    gFullFrameRequested = false;
    if (aClearScroll) {
        gScrollX = 0;
        gScrollY = 0;
    }
    gDirtyCount = 0;
    gPaintRegion = -1;
    gFrameRect = Rect(0, 0, aWidth, aHeight);
    gPaintRect = gFrameRect;
    gPaintClipRect = nullptr;
    gDrawableCount = 0;
    gDrawableOverflow = false;
    gActiveDrawable = nullptr;
    gActiveDrawableHasRect = false;
    gActiveDrawableSignature = 0;
    gActiveDrawableSignatureKnown = true;
}

void Reset() {
    gPhase = EIdle;
    gPreviousCount = 0;
    gCurrentCount = 0;
    gPreviousValid = false;
    gOverflow = false;
    gFullDependency = false;
    gDamageValid = false;
    gFullFrameRequested = false;
    gScrollX = 0;
    gScrollY = 0;
    gDirtyCount = 0;
    gPaintRegion = -1;
    gAdaptiveScore = 0;
    gAdaptiveFullFrames = 0;
    gAdaptiveFullPaintActive = false;
    gRecordSuppressionDepth = 0;
    gFastFullPaint = false;
    gDrawableCount = 0;
    gDrawableOverflow = false;
    gActiveDrawable = nullptr;
    gActiveDrawableHasRect = false;
    gActiveDrawableSignature = 0;
    gActiveDrawableSignatureKnown = true;
    gPreviousDrawableCount = 0;
    gPreviousDrawablesValid = false;
    gFrameRect = Rect();
    gPaintRect = Rect();
    gPaintClipRect = nullptr;
}

void ForceFullFrame(int aWidth, int aHeight) {
    Reset();
    gFrameRect = Rect(0, 0, aWidth, aHeight);
    ForceFull();
    gDamageValid = true;
}

void RequestFullFrame() {
    gFullFrameRequested = true;
    gScrollX = 0;
    gScrollY = 0;
}

bool FullFrameRequested() {
    return gFullFrameRequested;
}

void RequestScroll(int aShiftX, int aShiftY) {
    if (gFullFrameRequested)
        return;
    gScrollX += aShiftX;
    gScrollY += aShiftY;
}

bool PendingScroll(int& aShiftX, int& aShiftY) {
    aShiftX = gScrollX;
    aShiftY = gScrollY;
    return HasScroll();
}

void DiscardScroll() {
    gScrollX = 0;
    gScrollY = 0;
}

void BeginRecord(int aWidth, int aHeight) {
    ResetFrameState(ERecording, aWidth, aHeight, false);
}

void BeginScrollRecord(int aWidth, int aHeight) {
    BeginRecord(aWidth, aHeight);
}

void BeginFullPaintRecord(int aWidth, int aHeight) {
    ResetFrameState(EFullPaintRecording, aWidth, aHeight, true);
    ForceFull();
    gPaintRegion = 0;
}

void BeginFastFullPaint(int aWidth, int aHeight) {
    ResetFrameState(EFullPaintRecording, aWidth, aHeight, false);
    gPaintRegion = 0;
    gFastFullPaint = true;
    gRecordSuppressionDepth = 1;
}

void EndFastFullPaint() {
    if (gPhase != EFullPaintRecording || !gFastFullPaint)
        return;
    gRecordSuppressionDepth = 0;
    gFastFullPaint = false;
    if (gOverflow || gDrawableOverflow) {
        ForceFull();
        gScrollX = 0;
        gScrollY = 0;
    } else {
        DiffFrame();
    }
    SaveCommandBaseline();
    SaveDrawableBaseline();
    gDamageValid = true;
    gPaintRegion = -1;
    gPaintClipRect = nullptr;
    gPhase = EIdle;
}

bool IsFastFullPaint() {
    return gFastFullPaint;
}

void BeginDrawable(Drawable* aDrawable) {
    if (gPhase != ERecording && gPhase != EFullPaintRecording)
        return;
    gActiveDrawable = aDrawable;
    gActiveDrawableRect = Rect();
    gActiveDrawableHasRect = false;
    gActiveDrawableSignature = SymbianRenderDamage::HashSeed(0x44524157u);
    gActiveDrawableSignatureKnown = true;
}

void EndDrawable() {
    if ((gPhase != ERecording && gPhase != EFullPaintRecording) || !gActiveDrawable)
        return;
    if (gActiveDrawableHasRect) {
        if (gDrawableCount < KDrawableCapacity) {
            gDrawables[gDrawableCount].drawable = gActiveDrawable;
            gDrawables[gDrawableCount].rect = gActiveDrawableRect;
            gDrawables[gDrawableCount].signature = gActiveDrawableSignature;
            gDrawables[gDrawableCount].signatureKnown = gActiveDrawableSignatureKnown;
            ++gDrawableCount;
        } else {
            gDrawableOverflow = true;
        }
    }
    gActiveDrawable = nullptr;
    gActiveDrawableHasRect = false;
    gActiveDrawableSignature = 0;
    gActiveDrawableSignatureKnown = true;
}

bool IsRecording() {
    return gPhase == ERecording;
}

bool UseAdaptiveFullPaint() {
    if (gAdaptiveFullFrames <= 0)
        return false;
    --gAdaptiveFullFrames;
    return true;
}

void SetAdaptiveFullPaintActive(bool aActive) {
    gAdaptiveFullPaintActive = aActive;
}

bool AdaptiveFullPaintActive() {
    return gAdaptiveFullPaintActive;
}

void PushRecordSuppression() {
    ++gRecordSuppressionDepth;
}

void PopRecordSuppression() {
    if (gRecordSuppressionDepth > 0)
        --gRecordSuppressionDepth;
}

bool IsRecordSuppressed() {
    return gRecordSuppressionDepth > 0;
}

bool IsFullPaintRecording() {
    return gPhase == EFullPaintRecording;
}

void Record(unsigned int aSignature, const Rect& aInput) {
    if ((gPhase != ERecording && gPhase != EFullPaintRecording) || gOverflow)
        return;

    Rect rect = aInput;
    rect.Adjust(gFrameRect);
    if (rect.IsEmpty())
        return;

    if (gActiveDrawable) {
        TrackActiveDrawableRect(rect);
        if (gRecordSuppressionDepth == 0 ||
            (gFastFullPaint && gRecordSuppressionDepth == 1))
            TrackActiveDrawableSignature(aSignature);
        return;
    }

    if (gRecordSuppressionDepth > 0 && !(gFastFullPaint && gRecordSuppressionDepth == 1))
        return;

    if (gCurrentCount >= KCommandCapacity) {
        gOverflow = true;
        return;
    }

    gCurrent[gCurrentCount].signature = aSignature;
    gCurrent[gCurrentCount].rect = rect;
    ++gCurrentCount;
}

void MarkDirty(const Rect& aRect) {
    if (gPhase == ERecording)
        AddDirty(aRect);
}

void RequireFullPaint() {
    if (gPhase == ERecording)
        gFullDependency = true;
}

void EndRecord() {
    if (gPhase != ERecording)
        return;

    DiffFrame();
    UpdateAdaptiveMode();

    if (gFullDependency && gDirtyCount > 0)
        ForceFull();

    SaveCommandBaseline();
    SaveDrawableBaseline();

    gDamageValid = true;
    gPaintClipRect = nullptr;
    gPhase = EIdle;
}

void EndScrollRecord() {
    EndRecord();
}

void EndFullPaintRecord() {
    if (gPhase != EFullPaintRecording)
        return;

    SaveCommandBaseline();
    SaveDrawableBaseline();
    gDamageValid = true;
    gPaintRegion = -1;
    gPaintClipRect = nullptr;
    gPhase = EIdle;
}

bool NeedsPaint() {
    return gDirtyCount > 0;
}

bool IsFullFrameDirty() {
    return gDirtyCount == 1 && SameRect(gDirty[0], gFrameRect);
}

bool DamageValid() {
    return gDamageValid;
}

void ConsumeDamage() {
    gDamageValid = false;
    gScrollX = 0;
    gScrollY = 0;
}

int DirtyCount() {
    return gDirtyCount;
}

const Rect& DirtyRect(int aIndex) {
    return gDirty[aIndex];
}

void BeginPaint(int aDirtyIndex) {
    gPaintRegion = aDirtyIndex >= 0 && aDirtyIndex < gDirtyCount ? aDirtyIndex : -1;
    gPaintRect = gPaintRegion >= 0 ? gDirty[gPaintRegion] : gFrameRect;
    gPaintClipRect = SameRect(gPaintRect, gFrameRect) ? nullptr : &gPaintRect;
    gPhase = EPainting;
}

void EndPaint() {
    gPaintRegion = -1;
    gPaintClipRect = nullptr;
    gPhase = EIdle;
}

bool RecordedDrawablesValid() {
    return !gDrawableOverflow;
}

int RecordedDrawableCount() {
    return gDrawableCount;
}

Drawable* RecordedDrawable(int aIndex) {
    return aIndex >= 0 && aIndex < gDrawableCount ? gDrawables[aIndex].drawable : nullptr;
}

const Rect& RecordedDrawableRect(int aIndex) {
    static const Rect empty;
    return aIndex >= 0 && aIndex < gDrawableCount ? gDrawables[aIndex].rect : empty;
}

}

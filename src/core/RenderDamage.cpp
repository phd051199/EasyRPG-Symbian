#include "RenderDamage.h"

namespace {

enum TPhase {
    EIdle,
    ERecording,
    EPainting,
    EFullPaintRecording
};

const int KCommandCapacity = 2048;
const int KDirtyCapacity = 16;
const int KMaximumDirtyRegions = 4;
const int KFullRedrawAreaPercent = 88;
const int KDirtyRegionFixedCostDivisor = 12;
const int KCommandResyncLookahead = 8;

struct TCommand {
    unsigned int signature;
    Rect rect;
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
Rect gFrameRect;
Rect gPaintRect;
Rect gDirty[KDirtyCapacity];
int gDirtyCount = 0;
int gPaintRegion = -1;

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

bool SameCommand(const TCommand& aFirst, const TCommand& aSecond) {
    return aFirst.signature == aSecond.signature && SameRect(aFirst.rect, aSecond.rect);
}

void ForceFull() {
    gDirty[0] = gFrameRect;
    gDirtyCount = 1;
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

int DirtyArea() {
    int area = 0;
    for (int index = 0; index < gDirtyCount; ++index)
        area += Area(gDirty[index]);
    return area;
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

}

namespace SymbianRenderDamage {

void Reset() {
    gPhase = EIdle;
    gPreviousCount = 0;
    gCurrentCount = 0;
    gPreviousValid = false;
    gOverflow = false;
    gFullDependency = false;
    gDamageValid = false;
    gFullFrameRequested = false;
    gDirtyCount = 0;
    gPaintRegion = -1;
    gFrameRect = Rect();
    gPaintRect = Rect();
}

void ForceFullFrame(int aWidth, int aHeight) {
    gPhase = EIdle;
    gPreviousCount = 0;
    gCurrentCount = 0;
    gPreviousValid = false;
    gOverflow = false;
    gFullDependency = false;
    gFullFrameRequested = false;
    gFrameRect = Rect(0, 0, aWidth, aHeight);
    gPaintRect = gFrameRect;
    ForceFull();
    gDamageValid = true;
}

void RequestFullFrame() {
    gFullFrameRequested = true;
}

bool FullFrameRequested() {
    return gFullFrameRequested;
}

void BeginRecord(int aWidth, int aHeight) {
    gPhase = ERecording;
    gCurrentCount = 0;
    gOverflow = false;
    gFullDependency = false;
    gDamageValid = false;
    gFullFrameRequested = false;
    gDirtyCount = 0;
    gPaintRegion = -1;
    gFrameRect = Rect(0, 0, aWidth, aHeight);
    gPaintRect = gFrameRect;
}

void BeginFullPaintRecord(int aWidth, int aHeight) {
    gPhase = EFullPaintRecording;
    gCurrentCount = 0;
    gOverflow = false;
    gFullDependency = false;
    gDamageValid = false;
    gFullFrameRequested = false;
    gFrameRect = Rect(0, 0, aWidth, aHeight);
    gPaintRect = gFrameRect;
    ForceFull();
    gPaintRegion = 0;
}

bool IsRecording() {
    return gPhase == ERecording;
}

bool IsPainting() {
    return gPhase == EPainting || gPhase == EFullPaintRecording;
}

bool IsFullPaintRecording() {
    return gPhase == EFullPaintRecording;
}

void Record(unsigned int aSignature, const Rect& aInput) {
    if (gPhase != ERecording || gOverflow)
        return;

    Rect rect = aInput;
    rect.Adjust(gFrameRect);
    if (rect.IsEmpty())
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

    if (!gPreviousValid || gOverflow) {
        ForceFull();
    } else {
        int previousIndex = 0;
        int currentIndex = 0;
        while (previousIndex < gPreviousCount && currentIndex < gCurrentCount) {
            if (SameCommand(gPrevious[previousIndex], gCurrent[currentIndex])) {
                ++previousIndex;
                ++currentIndex;
                continue;
            }

            int previousMatch = -1;
            int currentMatch = -1;
            for (int offset = 1; offset <= KCommandResyncLookahead; ++offset) {
                if (previousMatch < 0 && previousIndex + offset < gPreviousCount &&
                    SameCommand(gPrevious[previousIndex + offset], gCurrent[currentIndex]))
                    previousMatch = offset;
                if (currentMatch < 0 && currentIndex + offset < gCurrentCount &&
                    SameCommand(gPrevious[previousIndex], gCurrent[currentIndex + offset]))
                    currentMatch = offset;
                if (previousMatch >= 0 || currentMatch >= 0)
                    break;
            }

            if (previousMatch >= 0 && (currentMatch < 0 || previousMatch <= currentMatch)) {
                for (int offset = 0; offset < previousMatch; ++offset)
                    AddDirty(gPrevious[previousIndex + offset].rect);
                previousIndex += previousMatch;
                continue;
            }

            if (currentMatch >= 0) {
                for (int offset = 0; offset < currentMatch; ++offset)
                    AddDirty(gCurrent[currentIndex + offset].rect);
                currentIndex += currentMatch;
                continue;
            }

            AddDirty(gPrevious[previousIndex].rect);
            AddDirty(gCurrent[currentIndex].rect);
            ++previousIndex;
            ++currentIndex;
        }

        while (previousIndex < gPreviousCount)
            AddDirty(gPrevious[previousIndex++].rect);
        while (currentIndex < gCurrentCount)
            AddDirty(gCurrent[currentIndex++].rect);
        OptimizeDirty();
    }

    if (gFullDependency && gDirtyCount > 0)
        ForceFull();

    if (!gOverflow) {
        TCommand* completed = gCurrent;
        gCurrent = gPrevious;
        gPrevious = completed;
        gPreviousCount = gCurrentCount;
        gPreviousValid = true;
    } else {
        gPreviousCount = 0;
        gPreviousValid = false;
    }

    gDamageValid = true;
    gPhase = EIdle;
}

void EndFullPaintRecord() {
    if (gPhase != EFullPaintRecording)
        return;

    gPreviousCount = 0;
    gPreviousValid = false;
    gDamageValid = true;
    gPaintRegion = -1;
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
}

bool IntersectsDirty(const Rect& aRect) {
    if (IsFullFrameDirty())
        return true;
    const int right = aRect.x + aRect.width;
    const int bottom = aRect.y + aRect.height;
    const int first = gPhase == EPainting && gPaintRegion >= 0 ? gPaintRegion : 0;
    const int last = gPhase == EPainting && gPaintRegion >= 0 ? gPaintRegion + 1 : gDirtyCount;
    for (int index = first; index < last; ++index) {
        const Rect& dirty = gDirty[index];
        const int dirtyRight = dirty.x + dirty.width;
        const int dirtyBottom = dirty.y + dirty.height;
        if (aRect.x < dirtyRight && dirty.x < right &&
            aRect.y < dirtyBottom && dirty.y < bottom)
            return true;
    }
    return false;
}

bool ContainsDirtyPoint(int aX, int aY) {
    if (IsFullFrameDirty())
        return true;
    const int first = gPhase == EPainting && gPaintRegion >= 0 ? gPaintRegion : 0;
    const int last = gPhase == EPainting && gPaintRegion >= 0 ? gPaintRegion + 1 : gDirtyCount;
    for (int index = first; index < last; ++index) {
        const Rect& dirty = gDirty[index];
        if (aX >= dirty.x && aX < dirty.x + dirty.width &&
            aY >= dirty.y && aY < dirty.y + dirty.height)
            return true;
    }
    return false;
}

int DirtyCount() {
    return gDirtyCount;
}

const Rect& DirtyRect(int aIndex) {
    return gDirty[aIndex];
}

const Rect& CurrentPaintRect() {
    if (gPhase == EPainting || gPhase == EFullPaintRecording)
        return gPaintRect;
    return gFrameRect;
}

void BeginPaint() {
    gPaintRegion = -1;
    gPaintRect = gDirtyCount > 0 ? gDirty[0] : gFrameRect;
    for (int index = 1; index < gDirtyCount; ++index)
        gPaintRect = UnionRect(gPaintRect, gDirty[index]);
    gPhase = EPainting;
}

void BeginPaint(int aDirtyIndex) {
    gPaintRegion = aDirtyIndex >= 0 && aDirtyIndex < gDirtyCount ? aDirtyIndex : -1;
    gPaintRect = gPaintRegion >= 0 ? gDirty[gPaintRegion] : gFrameRect;
    gPhase = EPainting;
}

void EndPaint() {
    gPaintRegion = -1;
    gPhase = EIdle;
}

}

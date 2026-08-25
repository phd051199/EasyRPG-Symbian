#ifndef EASYRPG_RENDER_DAMAGE_H
#define EASYRPG_RENDER_DAMAGE_H

#include "rect.h"

class Drawable;

namespace SymbianRenderDamage {

inline unsigned int HashValue(unsigned int aHash, unsigned int aValue)
{
    aHash ^= aValue + 0x9e3779b9u + (aHash << 6) + (aHash >> 2);
    aHash *= 16777619u;
    return aHash ? aHash : 1u;
}

inline unsigned int HashSeed(unsigned int aValue)
{
    return HashValue(2166136261u, aValue);
}

void Reset();
void ForceFullFrame(int aWidth, int aHeight);
void RequestFullFrame();
bool FullFrameRequested();
void RequestScroll(int aShiftX, int aShiftY);
bool PendingScroll(int& aShiftX, int& aShiftY);
void DiscardScroll();
void BeginRecord(int aWidth, int aHeight);
void BeginScrollRecord(int aWidth, int aHeight);
void BeginFullPaintRecord(int aWidth, int aHeight);
void BeginFastFullPaint(int aWidth, int aHeight);
void EndFastFullPaint();
bool IsFastFullPaint();
void BeginDrawable(Drawable* aDrawable);
void EndDrawable();
bool IsRecording();
bool UseAdaptiveFullPaint();
void SetAdaptiveFullPaintActive(bool aActive);
bool AdaptiveFullPaintActive();
void PushRecordSuppression();
void PopRecordSuppression();
bool IsRecordSuppressed();

class ScopedRecordSuppression
{
public:
    explicit ScopedRecordSuppression(bool aActive)
        : iActive(aActive)
    {
        if (iActive)
            PushRecordSuppression();
    }

    ~ScopedRecordSuppression()
    {
        if (iActive)
            PopRecordSuppression();
    }

private:
    ScopedRecordSuppression(const ScopedRecordSuppression&);
    ScopedRecordSuppression& operator=(const ScopedRecordSuppression&);
    bool iActive;
};
extern const Rect* gPaintClipRect;
inline const Rect* PaintClipRect()
{
    return gPaintClipRect;
}
bool IsFullPaintRecording();
void Record(unsigned int aSignature, const Rect& aRect);
void MarkDirty(const Rect& aRect);
void RequireFullPaint();
void EndRecord();
void EndScrollRecord();
void EndFullPaintRecord();
bool NeedsPaint();
bool IsFullFrameDirty();
bool DamageValid();
void ConsumeDamage();
int DirtyCount();
const Rect& DirtyRect(int aIndex);
void BeginPaint(int aDirtyIndex);
void EndPaint();
bool RecordedDrawablesValid();
int RecordedDrawableCount();
Drawable* RecordedDrawable(int aIndex);
const Rect& RecordedDrawableRect(int aIndex);

}

#endif

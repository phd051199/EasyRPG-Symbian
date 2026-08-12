#ifndef EASYRPG_RENDER_DAMAGE_H
#define EASYRPG_RENDER_DAMAGE_H

#include "rect.h"

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
void BeginRecord(int aWidth, int aHeight);
void BeginFullPaintRecord(int aWidth, int aHeight);
bool IsRecording();
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
void EndFullPaintRecord();
bool NeedsPaint();
bool IsFullFrameDirty();
bool DamageValid();
void ConsumeDamage();
int DirtyCount();
const Rect& DirtyRect(int aIndex);
void BeginPaint();
void BeginPaint(int aDirtyIndex);
void EndPaint();

}

#endif

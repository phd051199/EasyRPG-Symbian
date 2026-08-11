#ifndef EASYRPG_SYMBIAN_RENDER_DAMAGE_H
#define EASYRPG_SYMBIAN_RENDER_DAMAGE_H

#include "rect.h"

namespace SymbianRenderDamage {

void Reset();
void ForceFullFrame(int aWidth, int aHeight);
void BeginRecord(int aWidth, int aHeight);
bool IsRecording();
bool IsPainting();
void Record(unsigned long long aSignature, const Rect& aRect);
void RequireFullPaint();
void EndRecord();
bool NeedsPaint();
bool DamageValid();
void ConsumeDamage();
bool IntersectsDirty(const Rect& aRect);
bool ContainsDirtyPoint(int aX, int aY);
int DirtyCount();
const Rect& DirtyRect(int aIndex);
void BeginPaint();
void EndPaint();

}

#endif

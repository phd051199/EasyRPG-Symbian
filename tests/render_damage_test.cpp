#include <cassert>
#include "RenderDamage.h"

int main()
{
    SymbianRenderDamage::Reset();
    SymbianRenderDamage::BeginRecord(320, 240);
    SymbianRenderDamage::Record(1, Rect(0, 0, 320, 240));
    SymbianRenderDamage::Record(2, Rect(100, 100, 16, 16));
    SymbianRenderDamage::EndRecord();
    assert(SymbianRenderDamage::IsFullFrameDirty());
    SymbianRenderDamage::ConsumeDamage();

    SymbianRenderDamage::BeginRecord(320, 240);
    SymbianRenderDamage::Record(1, Rect(0, 0, 320, 240));
    SymbianRenderDamage::Record(2, Rect(98, 100, 16, 16));
    SymbianRenderDamage::EndRecord();
    assert(!SymbianRenderDamage::IsFullFrameDirty());
    assert(SymbianRenderDamage::DirtyCount() == 1);

    int shiftX = 0;
    int shiftY = 0;
    SymbianRenderDamage::RequestScroll(-2, 0);
    assert(SymbianRenderDamage::PendingScroll(shiftX, shiftY));
    assert(shiftX == -2 && shiftY == 0);
    SymbianRenderDamage::DiscardScroll();
    assert(!SymbianRenderDamage::PendingScroll(shiftX, shiftY));

    SymbianRenderDamage::RequestScroll(-2, 0);
    SymbianRenderDamage::RequestFullFrame();
    assert(!SymbianRenderDamage::PendingScroll(shiftX, shiftY));

    return 0;
}

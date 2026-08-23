#include "RenderDamage.h"

#include <cassert>
#include <cstddef>

int main() {
    Drawable* first = reinterpret_cast<Drawable*>(static_cast<std::size_t>(1));
    Drawable* second = reinterpret_cast<Drawable*>(static_cast<std::size_t>(2));

    SymbianRenderDamage::Reset();
    SymbianRenderDamage::BeginRecord(320, 240);
    SymbianRenderDamage::BeginDrawable(first);
    SymbianRenderDamage::Record(11u, Rect(4, 5, 10, 12));
    SymbianRenderDamage::EndDrawable();
    SymbianRenderDamage::BeginDrawable(second);
    SymbianRenderDamage::Record(21u, Rect(100, 80, 8, 8));
    SymbianRenderDamage::Record(22u, Rect(105, 84, 20, 10));
    SymbianRenderDamage::EndDrawable();
    SymbianRenderDamage::EndRecord();

    assert(SymbianRenderDamage::RecordedDrawablesValid());
    assert(SymbianRenderDamage::RecordedDrawableCount() == 2);
    assert(SymbianRenderDamage::RecordedDrawable(0) == first);
    assert(SymbianRenderDamage::RecordedDrawableRect(0) == Rect(4, 5, 10, 12));
    assert(SymbianRenderDamage::RecordedDrawable(1) == second);
    assert(SymbianRenderDamage::RecordedDrawableRect(1) == Rect(100, 80, 25, 14));

    SymbianRenderDamage::BeginRecord(320, 240);
    SymbianRenderDamage::BeginDrawable(first);
    SymbianRenderDamage::Record(11u, Rect(4, 5, 10, 12));
    SymbianRenderDamage::EndDrawable();
    SymbianRenderDamage::BeginDrawable(second);
    SymbianRenderDamage::Record(21u, Rect(100, 80, 8, 8));
    SymbianRenderDamage::Record(23u, Rect(105, 84, 20, 10));
    SymbianRenderDamage::EndDrawable();
    SymbianRenderDamage::EndRecord();

    assert(SymbianRenderDamage::NeedsPaint());
    assert(!SymbianRenderDamage::IsFullFrameDirty());
    assert(SymbianRenderDamage::DirtyCount() >= 1);
    assert(!SymbianRenderDamage::RecordedDrawableRect(0).IsOutOfBounds(Rect(0, 0, 32, 32)));
    assert(SymbianRenderDamage::RecordedDrawableRect(1).IsOutOfBounds(Rect(0, 0, 32, 32)));

    return 0;
}

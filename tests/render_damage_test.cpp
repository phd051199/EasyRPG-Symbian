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

    // Dense windows/sprites may emit hundreds of internal draw operations.
    // They must stay folded into the active drawable instead of overflowing
    // the small global-command stream and forcing a full repaint.
    SymbianRenderDamage::Reset();
    SymbianRenderDamage::BeginRecord(320, 240);
    SymbianRenderDamage::BeginDrawable(first);
    for (unsigned int index = 0; index < 600; ++index)
        SymbianRenderDamage::Record(1000u + index, Rect(index % 40, index % 30, 8, 8));
    SymbianRenderDamage::EndDrawable();
    SymbianRenderDamage::EndRecord();
    assert(SymbianRenderDamage::IsFullFrameDirty());

    SymbianRenderDamage::BeginRecord(320, 240);
    SymbianRenderDamage::BeginDrawable(first);
    for (unsigned int index = 0; index < 600; ++index)
        SymbianRenderDamage::Record(1000u + index, Rect(index % 40, index % 30, 8, 8));
    SymbianRenderDamage::EndDrawable();
    SymbianRenderDamage::EndRecord();
    assert(!SymbianRenderDamage::NeedsPaint());

    // During scroll, an unchanged drawable that moved by exactly the map
    // translation is already correct after the screen CopyRect. Only the
    // newly exposed strip should remain dirty.
    SymbianRenderDamage::Reset();
    SymbianRenderDamage::BeginRecord(320, 240);
    SymbianRenderDamage::BeginDrawable(first);
    SymbianRenderDamage::Record(77u, Rect(100, 80, 16, 16));
    SymbianRenderDamage::EndDrawable();
    SymbianRenderDamage::EndRecord();
    SymbianRenderDamage::ConsumeDamage();
    SymbianRenderDamage::RequestScroll(-2, 0);
    SymbianRenderDamage::BeginFastFullPaint(320, 240);
    SymbianRenderDamage::BeginDrawable(first);
    SymbianRenderDamage::Record(77u, Rect(98, 80, 16, 16));
    SymbianRenderDamage::EndDrawable();
    SymbianRenderDamage::EndFastFullPaint();
    assert(SymbianRenderDamage::NeedsPaint());
    assert(!SymbianRenderDamage::IsFullFrameDirty());
    assert(SymbianRenderDamage::DirtyCount() == 1);
    assert(SymbianRenderDamage::DirtyRect(0) == Rect(318, 0, 2, 240));

    // The primary scroll path records without repainting first. It uses the
    // shifted previous framebuffer as its baseline and should reach the same
    // exposed-strip result as the full-paint fallback above.
    SymbianRenderDamage::Reset();
    SymbianRenderDamage::BeginRecord(320, 240);
    SymbianRenderDamage::BeginDrawable(first);
    SymbianRenderDamage::Record(91u, Rect(120, 60, 16, 16));
    SymbianRenderDamage::EndDrawable();
    SymbianRenderDamage::EndRecord();
    SymbianRenderDamage::ConsumeDamage();
    SymbianRenderDamage::RequestScroll(0, 3);
    SymbianRenderDamage::BeginScrollRecord(320, 240);
    SymbianRenderDamage::BeginDrawable(first);
    SymbianRenderDamage::Record(91u, Rect(120, 63, 16, 16));
    SymbianRenderDamage::EndDrawable();
    SymbianRenderDamage::EndScrollRecord();
    assert(SymbianRenderDamage::DirtyCount() == 1);
    assert(SymbianRenderDamage::DirtyRect(0) == Rect(0, 0, 320, 3));

    // A transparent full-screen sprite can contain only centered text (for
    // example a chapter title). If that drawable disappears during a scroll,
    // its old pixels must be repainted. Full-frame records are not necessarily
    // tilemap backgrounds and must never be skipped solely because of size.
    SymbianRenderDamage::Reset();
    SymbianRenderDamage::BeginRecord(320, 240);
    SymbianRenderDamage::BeginDrawable(first);
    SymbianRenderDamage::Record(101u, Rect(0, 0, 320, 240));
    SymbianRenderDamage::EndDrawable();
    SymbianRenderDamage::BeginDrawable(second);
    SymbianRenderDamage::Record(102u, Rect(100, 80, 16, 16));
    SymbianRenderDamage::EndDrawable();
    SymbianRenderDamage::EndRecord();
    SymbianRenderDamage::ConsumeDamage();
    SymbianRenderDamage::RequestScroll(-2, 0);
    SymbianRenderDamage::BeginScrollRecord(320, 240);
    SymbianRenderDamage::BeginDrawable(second);
    SymbianRenderDamage::Record(102u, Rect(98, 80, 16, 16));
    SymbianRenderDamage::EndDrawable();
    SymbianRenderDamage::EndScrollRecord();
    assert(SymbianRenderDamage::IsFullFrameDirty());

    // Same case when the removed full-screen drawable was the final record.
    // The trailing-record cleanup must invalidate it as well.
    SymbianRenderDamage::Reset();
    SymbianRenderDamage::BeginRecord(320, 240);
    SymbianRenderDamage::BeginDrawable(first);
    SymbianRenderDamage::Record(111u, Rect(100, 80, 16, 16));
    SymbianRenderDamage::EndDrawable();
    SymbianRenderDamage::BeginDrawable(second);
    SymbianRenderDamage::Record(112u, Rect(0, 0, 320, 240));
    SymbianRenderDamage::EndDrawable();
    SymbianRenderDamage::EndRecord();
    SymbianRenderDamage::ConsumeDamage();
    SymbianRenderDamage::RequestScroll(-2, 0);
    SymbianRenderDamage::BeginScrollRecord(320, 240);
    SymbianRenderDamage::BeginDrawable(first);
    SymbianRenderDamage::Record(111u, Rect(98, 80, 16, 16));
    SymbianRenderDamage::EndDrawable();
    SymbianRenderDamage::EndScrollRecord();
    assert(SymbianRenderDamage::IsFullFrameDirty());

    // Full-screen commands outside Drawable use the same rule. A removed
    // command cannot be treated as a stable scrolling background by size.
    SymbianRenderDamage::Reset();
    SymbianRenderDamage::BeginRecord(320, 240);
    SymbianRenderDamage::Record(121u, Rect(0, 0, 320, 240));
    SymbianRenderDamage::EndRecord();
    SymbianRenderDamage::ConsumeDamage();
    SymbianRenderDamage::RequestScroll(0, 2);
    SymbianRenderDamage::BeginScrollRecord(320, 240);
    SymbianRenderDamage::EndScrollRecord();
    assert(SymbianRenderDamage::IsFullFrameDirty());

    return 0;
}

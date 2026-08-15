#include <cassert>

#include "RenderDamage.h"

namespace {

void RecordFrame(unsigned int baseSignature,
                 const Rect* rects,
                 int count,
                 bool mutate)
{
    SymbianRenderDamage::BeginRecord(320, 240);
    for (int index = 0; index < count; ++index) {
        unsigned int signature = baseSignature + static_cast<unsigned int>(index);
        if (mutate)
            signature += 1000u;
        SymbianRenderDamage::Record(signature, rects[index]);
    }
    SymbianRenderDamage::EndRecord();
}

void SeedFrame(const Rect* rects, int count)
{
    SymbianRenderDamage::Reset();
    RecordFrame(100u, rects, count, false);
    assert(SymbianRenderDamage::IsFullFrameDirty());
    SymbianRenderDamage::ConsumeDamage();
}

void TestUnchangedFrameHasNoDamage()
{
    const Rect rects[] = {
        Rect(8, 8, 16, 16),
        Rect(40, 40, 16, 16)
    };
    SeedFrame(rects, 2);
    RecordFrame(100u, rects, 2, false);
    assert(SymbianRenderDamage::DamageValid());
    assert(SymbianRenderDamage::DirtyCount() == 0);
}

void TestNearbyDamageUsesOnePaintRegion()
{
    const Rect rects[] = {
        Rect(10, 10, 8, 8),
        Rect(30, 10, 8, 8)
    };
    SeedFrame(rects, 2);
    RecordFrame(100u, rects, 2, true);
    assert(SymbianRenderDamage::DirtyCount() == 1);
    const Rect& dirty = SymbianRenderDamage::DirtyRect(0);
    assert(dirty.x == 10);
    assert(dirty.y == 10);
    assert(dirty.width == 28);
    assert(dirty.height == 8);
}

void TestScatteredDamageCapsPaintRegions()
{
    const Rect rects[] = {
        Rect(10, 10, 8, 8),
        Rect(30, 10, 8, 8),
        Rect(250, 200, 8, 8),
        Rect(270, 200, 8, 8)
    };
    SeedFrame(rects, 4);
    RecordFrame(100u, rects, 4, true);
    assert(SymbianRenderDamage::DirtyCount() > 0);
    assert(SymbianRenderDamage::DirtyCount() <= 2);
    assert(!SymbianRenderDamage::IsFullFrameDirty());
}

void TestFullDependencyFallsBackToFullFrame()
{
    const Rect rects[] = { Rect(16, 16, 16, 16) };
    SeedFrame(rects, 1);

    SymbianRenderDamage::BeginRecord(320, 240);
    SymbianRenderDamage::Record(999u, rects[0]);
    SymbianRenderDamage::RequireFullPaint();
    SymbianRenderDamage::EndRecord();

    assert(SymbianRenderDamage::IsFullFrameDirty());
}

}

int main()
{
    TestUnchangedFrameHasNoDamage();
    TestNearbyDamageUsesOnePaintRegion();
    TestScatteredDamageCapsPaintRegions();
    TestFullDependencyFallsBackToFullFrame();
    return 0;
}

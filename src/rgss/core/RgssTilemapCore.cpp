#include "rgss/core/RgssTilemapCore.h"

#include "rgss/core/RgssBitmapCore.h"
#include "rgss/core/RgssTypes.h"
#include "rgss/core/RgssVxAutotileTables.h"

namespace rgss {

namespace {

const int KTileSize = 32;

/* Canonical RPG Maker XP autotile quadrant table. Entries are 1-based
 * indices into a 6x8 grid of 16x16 quadrants in a 96x128 autotile frame. */
const unsigned char KAutotileTable[48][4] = {
    {27,28,33,34},{5,28,33,34},{27,6,33,34},{5,6,33,34},
    {27,28,33,12},{5,28,33,12},{27,6,33,12},{5,6,33,12},
    {27,28,11,34},{5,28,11,34},{27,6,11,34},{5,6,11,34},
    {27,28,11,12},{5,28,11,12},{27,6,11,12},{5,6,11,12},
    {25,26,31,32},{25,6,31,32},{25,26,31,12},{25,6,31,12},
    {15,16,21,22},{15,16,21,12},{15,16,11,22},{15,16,11,12},
    {29,30,35,36},{29,30,11,36},{5,30,35,36},{5,30,11,36},
    {39,40,45,46},{5,40,45,46},{39,6,45,46},{5,6,45,46},
    {25,30,31,36},{15,16,45,46},{13,14,19,20},{13,14,19,12},
    {17,18,23,24},{17,18,11,24},{41,42,47,48},{5,42,47,48},
    {37,38,43,44},{37,6,43,44},{13,18,19,24},{13,14,43,44},
    {37,42,43,48},{17,18,47,48},{13,18,43,48},{1,2,7,8}
};

int FloorDiv(int aValue, int aDivisor)
{
    int quotient = aValue / aDivisor;
    int remainder = aValue % aDivisor;
    if (remainder < 0)
        --quotient;
    return quotient;
}

} // namespace

RgssTilemapCore::RgssTilemapCore()
    : tileset(0), mapData(0), flashData(0), priorities(0), flags(0),
      ox(0), oy(0), generation(1), scratch(0), rowScratch(0), animationTick(0)
{
    for (int i = 0; i < 7; ++i)
        autotiles[i] = 0;
    for (int i = 0; i < 9; ++i)
        bitmaps[i] = 0;
}

RgssTilemapCore::~RgssTilemapCore()
{
    delete scratch;
    delete rowScratch;
}

RgssBitmapCore* RgssTilemapCore::Scratch(int aWidth, int aHeight)
{
    if (aWidth <= 0 || aHeight <= 0)
        return 0;
    if (!scratch)
        scratch = new RgssBitmapCore;
    if (!scratch->IsValid() || scratch->Width() != aWidth ||
        scratch->Height() != aHeight)
    {
        if (!scratch->CreateTransparent(aWidth, aHeight))
            return 0;
    }
    else
    {
        scratch->ClearTransparent();
    }
    return scratch;
}

RgssBitmapCore* RgssTilemapCore::RowScratch(int aWidth, int aHeight)
{
    if (aWidth <= 0 || aHeight <= 0)
        return 0;
    if (!rowScratch)
        rowScratch = new RgssBitmapCore;
    if (!rowScratch->IsValid() || rowScratch->Width() != aWidth ||
        rowScratch->Height() != aHeight)
    {
        if (!rowScratch->CreateTransparent(aWidth, aHeight))
            return 0;
    }
    else
    {
        rowScratch->ClearTransparent();
    }
    return rowScratch;
}

void RgssTilemapCore::UpdateAnimation()
{
    ++animationTick;
    if (animationTick > 0x3fffffff)
        animationTick = 0;
}

void RgssTilemapCore::DrawAutotile(RgssBitmapCore& aDest,
                                   RgssBitmapCore* aBitmap,
                                   int aPattern, int aX, int aY)
{
    if (!aBitmap || !aBitmap->IsValid())
        return;

    const int width = aBitmap->Width();
    const int height = aBitmap->Height();

    /* Several Essentials-era assets use a horizontal strip of complete
     * 32x32 frames instead of the stock 96x128 quadrant atlas. */
    if (height == 32 && width >= 32)
    {
        const int frames = width / 32;
        const int frame = frames > 0 ? (animationTick / 16) % frames : 0;
        aDest.Blit(aBitmap, Rect(frame * 32, 0, 32, 32), aX, aY, 255);
        return;
    }

    if (height < 128 || width < 96)
    {
        aDest.StretchBlt(aBitmap, Rect(0, 0, width, height), aX, aY,
                         32.0 / width, 32.0 / height, 255, false,
                         KBlendNormal);
        return;
    }

    if (aPattern < 0 || aPattern >= 48)
        aPattern = 0;
    const int frames = width / 96;
    const int frame = frames > 0 ? (animationTick / 16) % frames : 0;
    const int frameX = frame * 96;

    for (int quadrant = 0; quadrant < 4; ++quadrant)
    {
        const int index = KAutotileTable[aPattern][quadrant] - 1;
        const int sourceX = frameX + (index % 6) * 16;
        const int sourceY = (index / 6) * 16;
        const int destX = aX + (quadrant & 1) * 16;
        const int destY = aY + (quadrant >> 1) * 16;
        aDest.Blit(aBitmap, Rect(sourceX, sourceY, 16, 16),
                   destX, destY, 255);
    }
}

void RgssTilemapCore::DrawSheetTile(RgssBitmapCore& aDest,
                                    RgssBitmapCore* aBitmap,
                                    int aIndex, int aX, int aY)
{
    if (!aBitmap || !aBitmap->IsValid() || aIndex < 0)
        return;
    const int columns = aBitmap->Width() / KTileSize;
    if (columns <= 0)
        return;
    const int sourceX = (aIndex % columns) * KTileSize;
    const int sourceY = (aIndex / columns) * KTileSize;
    if (sourceX + KTileSize > aBitmap->Width() ||
        sourceY + KTileSize > aBitmap->Height())
        return;
    aDest.Blit(aBitmap, Rect(sourceX, sourceY, KTileSize, KTileSize),
               aX, aY, 255);
}

void RgssTilemapCore::DrawVxAutotile(RgssBitmapCore& aDest,
                                     int aTileId, int aX, int aY)
{
    const int kA1 = 2048;
    const int kA2 = 2816;
    const int kA3 = 4352;
    const int kA4 = 5888;
    const int kMax = 8192;
    if (aTileId < kA1 || aTileId >= kMax)
        return;

    const int kind = (aTileId - kA1) / 48;
    const int shape = (aTileId - kA1) % 48;
    const int tx = kind % 8;
    const int ty = kind / 8;
    const int animationFrame = animationTick / 30;
    const int waterSurfaceIndex[4] = {0, 1, 2, 1};

    RgssBitmapCore* bitmap = 0;
    enum AutotileTableKind { KFloorTable, KWallTable, KWaterfallTable };
    AutotileTableKind tableKind = KFloorTable;
    int tablePatterns = 48;
    int bx = 0;
    int by = 0;

    if (aTileId < kA2)
    {
        bitmap = bitmaps[0];
        if (kind == 0)
        {
            bx = waterSurfaceIndex[animationFrame & 3] * 2;
            by = 0;
        }
        else if (kind == 1)
        {
            bx = waterSurfaceIndex[animationFrame & 3] * 2;
            by = 3;
        }
        else if (kind == 2)
        {
            bx = 6;
            by = 0;
        }
        else if (kind == 3)
        {
            bx = 6;
            by = 3;
        }
        else
        {
            bx = (tx / 4) * 8;
            by = ty * 6 + ((tx / 2) & 1) * 3;
            if ((kind & 1) == 0)
            {
                bx += waterSurfaceIndex[animationFrame & 3] * 2;
            }
            else
            {
                bx += 6;
                by += animationFrame % 3;
                tableKind = KWaterfallTable;
                tablePatterns = 4;
            }
        }
    }
    else if (aTileId < kA3)
    {
        bitmap = bitmaps[1];
        bx = tx * 2;
        by = (ty - 2) * 3;
    }
    else if (aTileId < kA4)
    {
        bitmap = bitmaps[2];
        bx = tx * 2;
        by = (ty - 6) * 2;
        tableKind = KWallTable;
        tablePatterns = 16;
    }
    else
    {
        bitmap = bitmaps[3];
        bx = tx * 2;
        const int relativeY = ty - 10;
        by = (relativeY * 5 + ((ty & 1) ? 1 : 0)) / 2;
        if (ty & 1)
        {
            tableKind = KWallTable;
            tablePatterns = 16;
        }
    }

    if (!bitmap || !bitmap->IsValid())
        return;

    int tableShape = shape;
    if (tablePatterns == 16)
        tableShape &= 15;
    else if (tablePatterns == 4)
        tableShape &= 3;

    const signed char (*quarters)[2] = 0;
    if (tableKind == KWallTable)
        quarters = KVxWallAutotileTable[tableShape];
    else if (tableKind == KWaterfallTable)
        quarters = KVxWaterfallAutotileTable[tableShape];
    else
        quarters = KVxFloorAutotileTable[tableShape];

    for (int quadrant = 0; quadrant < 4; ++quadrant)
    {
        const int sourceQuarterX = bx * 2 + quarters[quadrant][0];
        const int sourceQuarterY = by * 2 + quarters[quadrant][1];
        const int sourceX = sourceQuarterX * 16;
        const int sourceY = sourceQuarterY * 16;
        if (sourceX < 0 || sourceY < 0 ||
            sourceX + 16 > bitmap->Width() || sourceY + 16 > bitmap->Height())
            continue;
        const int destX = aX + (quadrant & 1) * 16;
        const int destY = aY + (quadrant >> 1) * 16;
        aDest.Blit(bitmap, Rect(sourceX, sourceY, 16, 16),
                   destX, destY, 255);
    }
}

void RgssTilemapCore::DrawVxTile(RgssBitmapCore& aDest, int aTileId,
                                 int aX, int aY)
{
    if (aTileId <= 0)
        return;

    /* VX/VX Ace B-E pages are 256 tile-id blocks. Use the source bitmap's
     * actual column count so both 256- and 512-pixel sheet widths work. */
    if (aTileId < 1024)
    {
        const int page = aTileId / 256;
        const int bitmapIndex = 5 + page;
        if (bitmapIndex >= 5 && bitmapIndex < 9)
            DrawSheetTile(aDest, bitmaps[bitmapIndex], aTileId % 256, aX, aY);
        return;
    }

    if (aTileId >= 1536 && aTileId < 1664)
    {
        DrawSheetTile(aDest, bitmaps[4], aTileId - 1536, aX, aY);
        return;
    }

    if (aTileId >= 2048 && aTileId < 8192)
    {
        DrawVxAutotile(aDest, aTileId, aX, aY);
        return;
    }
}

void RgssTilemapCore::DrawTile(RgssBitmapCore& aDest, int aTileId,
                               int aX, int aY)
{
    if (generation >= 2)
    {
        DrawVxTile(aDest, aTileId, aX, aY);
        return;
    }

    if (aTileId <= 0)
        return;

    if (aTileId < 384)
    {
        if (aTileId < 48)
            return;
        const int autotileIndex = aTileId / 48 - 1;
        if (autotileIndex < 0 || autotileIndex >= 7)
            return;
        DrawAutotile(aDest, autotiles[autotileIndex], aTileId % 48, aX, aY);
        return;
    }

    if (!tileset || !tileset->IsValid())
        return;
    const int index = aTileId - 384;
    const int sourceX = (index % 8) * KTileSize;
    const int sourceY = (index / 8) * KTileSize;
    if (sourceX + KTileSize > tileset->Width() ||
        sourceY + KTileSize > tileset->Height())
        return;
    aDest.Blit(tileset, Rect(sourceX, sourceY, KTileSize, KTileSize),
               aX, aY, 255);
}

int RgssTilemapCore::PriorityForTile(int aTileId) const
{
    if (aTileId <= 0)
        return 0;
    if (generation >= 2)
    {
        if (!flags)
            return 0;
        short rawFlags = 0;
        if (!flags->Get(aTileId, 0, 0, &rawFlags))
            return 0;
        return (static_cast<unsigned short>(rawFlags) & 0x10u) != 0 ? 1 : 0;
    }
    if (!priorities)
        return 0;
    short priority = 0;
    if (!priorities->Get(aTileId, 0, 0, &priority))
        return 0;
    if (priority < 0) return 0;
    if (priority > 5) return 5;
    return priority;
}

void RgssTilemapCore::VisibleRows(int aViewportY, const Rect& aClip,
                                  int* aFirstRow, int* aLastRow) const
{
    int first = 0;
    int last = -1;
    if (mapData && mapData->Ysize() > 0 && !aClip.IsEmpty())
    {
        const int worldTop = aClip.y - aViewportY + oy;
        first = FloorDiv(worldTop, KTileSize);
        last = FloorDiv(worldTop + aClip.height - 1, KTileSize);
        if (first < 0) first = 0;
        if (last >= mapData->Ysize()) last = mapData->Ysize() - 1;
        if (first > last) { first = 0; last = -1; }
    }
    if (aFirstRow) *aFirstRow = first;
    if (aLastRow) *aLastRow = last;
}

void RgssTilemapCore::DrawBaseSlice(RgssBitmapCore& aDest, int aViewportX,
                                    int aViewportY, const Rect& aClip)
{
    if (!mapData || aClip.IsEmpty() || mapData->Xsize() <= 0 ||
        mapData->Ysize() <= 0 || mapData->Zsize() <= 0)
        return;

    RgssBitmapCore* target = Scratch(aClip.width, aClip.height);
    if (!target)
        return;

    const int worldLeft = aClip.x - aViewportX + ox;
    int firstMapX = FloorDiv(worldLeft, KTileSize);
    int lastMapX = FloorDiv(worldLeft + aClip.width - 1, KTileSize);
    if (firstMapX < 0) firstMapX = 0;
    if (lastMapX >= mapData->Xsize()) lastMapX = mapData->Xsize() - 1;
    int firstMapY = 0, lastMapY = -1;
    VisibleRows(aViewportY, aClip, &firstMapY, &lastMapY);
    if (firstMapX > lastMapX || firstMapY > lastMapY)
        return;

    for (int layer = 0; layer < mapData->Zsize(); ++layer)
    {
        for (int mapY = firstMapY; mapY <= lastMapY; ++mapY)
        {
            for (int mapX = firstMapX; mapX <= lastMapX; ++mapX)
            {
                short rawTileId = 0;
                if (!mapData->Get(mapX, mapY, layer, &rawTileId))
                    continue;
                const int tileId = static_cast<unsigned short>(rawTileId);
                if (PriorityForTile(tileId) != 0)
                    continue;
                const int screenX = aViewportX + mapX * KTileSize - ox;
                const int screenY = aViewportY + mapY * KTileSize - oy;
                DrawTile(*target, tileId,
                         screenX - aClip.x, screenY - aClip.y);
            }
        }
    }
    aDest.Blit(target, Rect(0, 0, aClip.width, aClip.height),
               aClip.x, aClip.y, 255);
}

void RgssTilemapCore::DrawPriorityRow(RgssBitmapCore& aDest, int aViewportX,
                                      int aViewportY, const Rect& aClip,
                                      int aMapY, int aPriority)
{
    if (!mapData || aPriority < 1 || aPriority > 5 ||
        aMapY < 0 || aMapY >= mapData->Ysize())
        return;

    const int screenY = aViewportY + aMapY * KTileSize - oy;
    const int top = screenY > aClip.y ? screenY : aClip.y;
    const int tileBottom = screenY + KTileSize;
    const int clipBottom = aClip.y + aClip.height;
    const int bottom = tileBottom < clipBottom ? tileBottom : clipBottom;
    if (top >= bottom)
        return;

    RgssBitmapCore* target = RowScratch(aClip.width, bottom - top);
    if (!target)
        return;

    const int worldLeft = aClip.x - aViewportX + ox;
    int firstMapX = FloorDiv(worldLeft, KTileSize);
    int lastMapX = FloorDiv(worldLeft + aClip.width - 1, KTileSize);
    if (firstMapX < 0) firstMapX = 0;
    if (lastMapX >= mapData->Xsize()) lastMapX = mapData->Xsize() - 1;
    if (firstMapX > lastMapX)
        return;

    for (int layer = 0; layer < mapData->Zsize(); ++layer)
    {
        for (int mapX = firstMapX; mapX <= lastMapX; ++mapX)
        {
            short rawTileId = 0;
            if (!mapData->Get(mapX, aMapY, layer, &rawTileId))
                continue;
            const int tileId = static_cast<unsigned short>(rawTileId);
            if (PriorityForTile(tileId) != aPriority)
                continue;
            const int screenX = aViewportX + mapX * KTileSize - ox;
            DrawTile(*target, tileId, screenX - aClip.x, screenY - top);
        }
    }

    aDest.Blit(target, Rect(0, 0, aClip.width, bottom - top),
               aClip.x, top, 255);
}

int RgssTilemapCore::DrawSliceCount(int, int aViewportY,
                                    const Rect& aClip) const
{
    if (generation >= 2)
        return flags ? 2 : 1;
    if (!priorities)
        return 1;
    int first = 0, last = -1;
    VisibleRows(aViewportY, aClip, &first, &last);
    return last >= first ? 1 + (last - first + 1) * 5 : 1;
}

void RgssTilemapCore::DrawSliceSortKey(int aSlice, int, int aViewportY,
                                       const Rect& aClip, int* aZ, int* aY) const
{
    if (aSlice <= 0 || (generation >= 2 ? !flags : !priorities))
    {
        if (aZ) *aZ = z;
        if (aY) *aY = 0;
        return;
    }
    if (generation >= 2)
    {
        if (aZ) *aZ = z + 200;
        if (aY) *aY = 0;
        return;
    }
    int first = 0, last = -1;
    VisibleRows(aViewportY, aClip, &first, &last);
    const int relative = (aSlice - 1) / 5;
    const int priority = (aSlice - 1) % 5 + 1;
    const int mapY = first + relative;
    if (mapY > last)
    {
        if (aZ) *aZ = z;
        if (aY) *aY = 0;
        return;
    }
    const int screenY = mapY * KTileSize - oy;
    if (aZ) *aZ = z + screenY + (priority + 1) * KTileSize;
    if (aY) *aY = screenY;
}

void RgssTilemapCore::DrawSlice(RgssBitmapCore& aDest, int aViewportX,
                                int aViewportY, const Rect& aClip, int aSlice)
{
    if (aSlice <= 0 || (generation >= 2 ? !flags : !priorities))
    {
        DrawBaseSlice(aDest, aViewportX, aViewportY, aClip);
        return;
    }
    if (generation >= 2)
    {
        int first = 0, last = -1;
        VisibleRows(aViewportY, aClip, &first, &last);
        for (int mapY = first; mapY <= last; ++mapY)
            DrawPriorityRow(aDest, aViewportX, aViewportY, aClip, mapY, 1);
        return;
    }
    int first = 0, last = -1;
    VisibleRows(aViewportY, aClip, &first, &last);
    const int mapY = first + (aSlice - 1) / 5;
    const int priority = (aSlice - 1) % 5 + 1;
    if (mapY <= last)
        DrawPriorityRow(aDest, aViewportX, aViewportY, aClip,
                        mapY, priority);
}

void RgssTilemapCore::Draw(RgssBitmapCore& aDest, int aViewportX,
                           int aViewportY, const Rect& aClip)
{
    DrawBaseSlice(aDest, aViewportX, aViewportY, aClip);
    int first = 0, last = -1;
    if (generation >= 2)
    {
        if (!flags)
            return;
        VisibleRows(aViewportY, aClip, &first, &last);
        for (int mapY = first; mapY <= last; ++mapY)
            DrawPriorityRow(aDest, aViewportX, aViewportY, aClip, mapY, 1);
        return;
    }
    if (!priorities)
        return;
    VisibleRows(aViewportY, aClip, &first, &last);
    for (int mapY = first; mapY <= last; ++mapY)
        for (int priority = 1; priority <= 5; ++priority)
            DrawPriorityRow(aDest, aViewportX, aViewportY, aClip,
                            mapY, priority);
}

} // namespace rgss

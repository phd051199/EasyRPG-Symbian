#ifndef RGSS_TILEMAP_CORE_H
#define RGSS_TILEMAP_CORE_H

#include "rgss/core/RgssScene.h"

namespace rgss {

class RgssBitmapCore;
class Table;

class RgssTilemapCore : public SceneElement
{
public:
    RgssTilemapCore();
    ~RgssTilemapCore();

    RgssBitmapCore* tileset;
    RgssBitmapCore* autotiles[7];
    RgssBitmapCore* bitmaps[9];
    Table* mapData;
    Table* flashData;
    Table* priorities;
    Table* flags;
    int ox;
    int oy;
    int generation;

    void UpdateAnimation();
    void Draw(RgssBitmapCore& aDest, int aViewportX, int aViewportY,
              const Rect& aClip);
    int DrawSliceCount(int aViewportX, int aViewportY,
                       const Rect& aClip) const;
    void DrawSliceSortKey(int aSlice, int aViewportX, int aViewportY,
                          const Rect& aClip, int* aZ, int* aY) const;
    void DrawSlice(RgssBitmapCore& aDest, int aViewportX, int aViewportY,
                   const Rect& aClip, int aSlice);

private:
    RgssBitmapCore* Scratch(int aWidth, int aHeight);
    RgssBitmapCore* RowScratch(int aWidth, int aHeight);
    void DrawTile(RgssBitmapCore& aDest, int aTileId, int aX, int aY);
    void DrawVxTile(RgssBitmapCore& aDest, int aTileId, int aX, int aY);
    void DrawVxAutotile(RgssBitmapCore& aDest, int aTileId, int aX, int aY);
    void DrawSheetTile(RgssBitmapCore& aDest, RgssBitmapCore* aBitmap,
                       int aIndex, int aX, int aY);
    void DrawAutotile(RgssBitmapCore& aDest, RgssBitmapCore* aBitmap,
                      int aPattern, int aX, int aY);
    int PriorityForTile(int aTileId) const;
    bool HasPriorityTable() const;
    void VisibleRows(int aViewportY, const Rect& aClip,
                     int* aFirstRow, int* aLastRow) const;
    void DrawBaseSlice(RgssBitmapCore& aDest, int aViewportX, int aViewportY,
                       const Rect& aClip);
    void DrawPriorityRow(RgssBitmapCore& aDest, int aViewportX, int aViewportY,
                         const Rect& aClip, int aMapY, int aPriority);

    RgssBitmapCore* scratch;
    RgssBitmapCore* rowScratch;
    int animationTick;
};

} // namespace rgss

#endif

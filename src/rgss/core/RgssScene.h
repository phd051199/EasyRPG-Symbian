#ifndef RGSS_SCENE_H
#define RGSS_SCENE_H

/*
 * RGSS1 scene graph. The scene owns nothing: viewports and elements are
 * allocated by the binding, registered with AddViewport/AddElement, and
 * must be removed before being deleted. The default (nil) viewport is the
 * screen itself and is owned by the scene.
 */

#include <vector>

#include "rgss/core/RgssTypes.h"

namespace rgss {

class RgssBitmapCore;
class RgssScene;
class ViewportElement;

/* Base of everything drawable inside a viewport (Sprite now; Window, Plane
 * and Tilemap later). Draw order inside a viewport is (z asc, sprite y asc,
 * insertion order) - the Enterbrain tie-break quirk. */
class SceneElement
{
public:
    SceneElement();
    virtual ~SceneElement() {}

    bool visible;
    int z;
    bool disposed;
    ViewportElement* viewport; /* null = default (screen) viewport */

    /* Sprites report their y for the tie-break; Window/Plane/Tilemap use 0. */
    virtual int SortY() const { return 0; }

    /* aViewportX/aViewportY are the viewport origin already offset by
     * ox/oy, in screen coordinates; aClip is the viewport clip rectangle. */
    virtual void Draw(RgssBitmapCore& aDest, int aViewportX, int aViewportY,
                      const Rect& aClip) = 0;

    /* Most drawables are one scene-sort item. Tilemap overrides these to
     * expose priority rows as independent sort slices without coupling the
     * scene graph to Tilemap itself. */
    virtual int DrawSliceCount(int aViewportX, int aViewportY,
                               const Rect& aClip) const;
    virtual void DrawSliceSortKey(int aSlice, int aViewportX, int aViewportY,
                                  const Rect& aClip, int* aZ, int* aY) const;
    virtual void DrawSlice(RgssBitmapCore& aDest, int aViewportX,
                           int aViewportY, const Rect& aClip, int aSlice);

    /* Managed by RgssScene::AddElement; also the final sort tie-break. */
    unsigned long insertionOrder;
};

class ViewportElement
{
public:
    explicit ViewportElement(const Rect& aRect);

    Rect rect; /* screen-space position and size */
    int ox, oy;
    int z;
    bool visible;
    bool disposed;
    Color color;  /* stored here; applied to pixels from M-R5 */
    Tone tone;    /* stored here; applied to pixels from M-R5 */
    Color flashColor; /* flash state stored; applied from M-R5 */

    std::vector<SceneElement*> elements; /* not owned */

    /* Managed by RgssScene::AddViewport; also the viewport sort tie-break. */
    unsigned long insertionOrder;

private:
    friend class RgssScene;
    ViewportElement() : ox(0), oy(0), z(0), visible(true), disposed(false),
                        insertionOrder(0) {}
};

struct SceneDrawSlice
{
    SceneElement* element;
    int slice;
    int z;
    int y;
    unsigned long insertionOrder;
};

class RgssScene
{
public:
    RgssScene();

    /* The screen viewport (nil viewport); rect tracks the screen size. */
    ViewportElement* DefaultViewport();

    void AddViewport(ViewportElement* aViewport);
    void RemoveViewport(ViewportElement* aViewport);
    /* Uses aElement->viewport (null = default viewport). */
    void AddElement(SceneElement* aElement);
    void RemoveElement(SceneElement* aElement);
    void SetScreenSize(int aWidth, int aHeight);

    /* Draws all visible viewports/elements onto aScreen (z asc order). */
    void Compose(RgssBitmapCore& aScreen);

private:
    RgssScene(const RgssScene&);
    RgssScene& operator=(const RgssScene&);

    ViewportElement screenViewport;
    std::vector<ViewportElement*> viewports; /* explicit ones, not owned */
    std::vector<ViewportElement*> viewportScratch;
    std::vector<SceneDrawSlice> drawScratch;
    unsigned long insertionCounter;
};

} // namespace rgss

#endif

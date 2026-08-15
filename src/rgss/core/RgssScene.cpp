#include "rgss/core/RgssScene.h"

#include <algorithm>

#include "rgss/core/RgssBitmapCore.h"

namespace rgss {

SceneElement::SceneElement()
    : visible(true), z(0), disposed(false), viewport(0), insertionOrder(0)
{
}

int SceneElement::DrawSliceCount(int, int, const Rect&) const
{
    return 1;
}

void SceneElement::DrawSliceSortKey(int, int, int, const Rect&,
                                    int* aZ, int* aY) const
{
    if (aZ) *aZ = z;
    if (aY) *aY = SortY();
}

void SceneElement::DrawSlice(RgssBitmapCore& aDest, int aViewportX,
                             int aViewportY, const Rect& aClip, int aSlice)
{
    if (aSlice == 0)
        Draw(aDest, aViewportX, aViewportY, aClip);
}

ViewportElement::ViewportElement(const Rect& aRect)
    : rect(aRect), ox(0), oy(0), z(0), visible(true), disposed(false),
      insertionOrder(0)
{
}

RgssScene::RgssScene() : insertionCounter(0)
{
    screenViewport.rect.Set(0, 0, 640, 480);
    screenViewport.insertionOrder = insertionCounter++;
}

ViewportElement* RgssScene::DefaultViewport()
{
    return &screenViewport;
}

void RgssScene::AddViewport(ViewportElement* aViewport)
{
    if (!aViewport)
        return;
    aViewport->insertionOrder = insertionCounter++;
    viewports.push_back(aViewport);
}

void RgssScene::RemoveViewport(ViewportElement* aViewport)
{
    if (!aViewport)
        return;
    /* Detach the viewport's elements so stale pointers never Compose. */
    for (size_t i = 0; i < aViewport->elements.size(); ++i)
        aViewport->elements[i]->viewport = 0;
    for (size_t i = 0; i < viewports.size(); ++i) {
        if (viewports[i] == aViewport) {
            viewports.erase(viewports.begin() + i);
            break;
        }
    }
}

void RgssScene::AddElement(SceneElement* aElement)
{
    if (!aElement)
        return;
    aElement->insertionOrder = insertionCounter++;
    if (aElement->viewport)
        aElement->viewport->elements.push_back(aElement);
    else
        screenViewport.elements.push_back(aElement);
}

void RgssScene::RemoveElement(SceneElement* aElement)
{
    if (!aElement)
        return;
    ViewportElement* owner = aElement->viewport ? aElement->viewport
                                                : &screenViewport;
    std::vector<SceneElement*>& list = owner->elements;
    for (size_t i = 0; i < list.size(); ++i) {
        if (list[i] == aElement) {
            list.erase(list.begin() + i);
            break;
        }
    }
}

void RgssScene::SetScreenSize(int aWidth, int aHeight)
{
    screenViewport.rect.Set(0, 0, aWidth, aHeight);
}

namespace {

bool ViewportLess(const ViewportElement* a, const ViewportElement* b)
{
    if (a->z != b->z)
        return a->z < b->z;
    return a->insertionOrder < b->insertionOrder;
}

bool DrawSliceLess(const SceneDrawSlice& a, const SceneDrawSlice& b)
{
    if (a.z != b.z)
        return a.z < b.z;
    if (a.y != b.y)
        return a.y < b.y;
    if (a.insertionOrder != b.insertionOrder)
        return a.insertionOrder < b.insertionOrder;
    return a.slice < b.slice;
}

} // namespace

void RgssScene::Compose(RgssBitmapCore& aScreen)
{
    if (!aScreen.IsValid())
        return;
    int screenW = aScreen.Width();
    int screenH = aScreen.Height();

    viewportScratch.clear();
    viewportScratch.push_back(&screenViewport);
    for (size_t i = 0; i < viewports.size(); ++i)
        viewportScratch.push_back(viewports[i]);
    std::stable_sort(viewportScratch.begin(), viewportScratch.end(), ViewportLess);

    for (size_t v = 0; v < viewportScratch.size(); ++v) {
        ViewportElement* viewport = viewportScratch[v];
        if (!viewport->visible || viewport->disposed)
            continue;
        /* Clip the viewport to the screen. */
        int x1 = viewport->rect.x > 0 ? viewport->rect.x : 0;
        int y1 = viewport->rect.y > 0 ? viewport->rect.y : 0;
        int x2 = viewport->rect.x + viewport->rect.width < screenW
                     ? viewport->rect.x + viewport->rect.width
                     : screenW;
        int y2 = viewport->rect.y + viewport->rect.height < screenH
                     ? viewport->rect.y + viewport->rect.height
                     : screenH;
        if (x1 >= x2 || y1 >= y2)
            continue;
        Rect clip(x1, y1, x2 - x1, y2 - y1);
        int originX = viewport->rect.x - viewport->ox;
        int originY = viewport->rect.y - viewport->oy;

        drawScratch.clear();
        for (size_t i = 0; i < viewport->elements.size(); ++i) {
            SceneElement* element = viewport->elements[i];
            if (!element || !element->visible || element->disposed)
                continue;
            int count = element->DrawSliceCount(originX, originY, clip);
            if (count < 0)
                count = 0;
            for (int slice = 0; slice < count; ++slice) {
                SceneDrawSlice command;
                command.element = element;
                command.slice = slice;
                command.z = element->z;
                command.y = element->SortY();
                command.insertionOrder = element->insertionOrder;
                element->DrawSliceSortKey(slice, originX, originY, clip,
                                          &command.z, &command.y);
                drawScratch.push_back(command);
            }
        }
        std::stable_sort(drawScratch.begin(), drawScratch.end(), DrawSliceLess);

        for (size_t i = 0; i < drawScratch.size(); ++i) {
            SceneDrawSlice& command = drawScratch[i];
            if (!command.element->visible || command.element->disposed)
                continue;
            command.element->DrawSlice(aScreen, originX, originY, clip,
                                       command.slice);
        }
    }
}

} // namespace rgss

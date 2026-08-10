#include <stdlib.h>
#include <string.h>

#include "pixman-private.h"

pixman_implementation_t *global_implementation = 0;

static uint32_t ReadPixel32(bits_image_t* image, int x, int y) {
    uint8_t* row = (uint8_t*)image->bits + image->rowstride * 4 * y;
    switch (image->format) {
    case PIXMAN_a8r8g8b8:
        return ((uint32_t*)row)[x];
    case PIXMAN_x8r8g8b8:
        return ((uint32_t*)row)[x] | 0xff000000u;
    case PIXMAN_a8b8g8r8: {
        uint32_t p = ((uint32_t*)row)[x];
        return (p & 0xff000000u) | ((p & 0x000000ffu) << 16) | (p & 0x0000ff00u) | ((p & 0x00ff0000u) >> 16);
    }
    case PIXMAN_x8b8g8r8: {
        uint32_t p = ((uint32_t*)row)[x];
        return 0xff000000u | ((p & 0x000000ffu) << 16) | (p & 0x0000ff00u) | ((p & 0x00ff0000u) >> 16);
    }
    case PIXMAN_b8g8r8a8: {
        uint32_t p = ((uint32_t*)row)[x];
        return ((p & 0x000000ffu) << 24) | ((p & 0x0000ff00u) << 8) | ((p & 0x00ff0000u) >> 8) | ((p & 0xff000000u) >> 24);
    }
    case PIXMAN_b8g8r8x8: {
        uint32_t p = ((uint32_t*)row)[x];
        return 0xff000000u | ((p & 0x0000ff00u) << 8) | ((p & 0x00ff0000u) >> 8) | ((p & 0xff000000u) >> 24);
    }
    case PIXMAN_r8g8b8a8: {
        uint32_t p = ((uint32_t*)row)[x];
        return ((p & 0x000000ffu) << 24) | ((p & 0xff000000u) >> 8) | ((p & 0x00ff0000u) >> 8) | ((p & 0x0000ff00u) >> 8);
    }
    case PIXMAN_r8g8b8x8: {
        uint32_t p = ((uint32_t*)row)[x];
        return 0xff000000u | ((p & 0xff000000u) >> 8) | ((p & 0x00ff0000u) >> 8) | ((p & 0x0000ff00u) >> 8);
    }
    case PIXMAN_a8:
        return ((uint32_t)row[x]) << 24;
    case PIXMAN_r5g6b5: {
        uint16_t p = ((uint16_t*)row)[x];
        uint32_t r = (p >> 11) & 31;
        uint32_t g = (p >> 5) & 63;
        uint32_t b = p & 31;
        r = (r << 3) | (r >> 2);
        g = (g << 2) | (g >> 4);
        b = (b << 3) | (b >> 2);
        return 0xff000000u | (r << 16) | (g << 8) | b;
    }
    case PIXMAN_b5g6r5: {
        uint16_t p = ((uint16_t*)row)[x];
        uint32_t b = (p >> 11) & 31;
        uint32_t g = (p >> 5) & 63;
        uint32_t r = p & 31;
        r = (r << 3) | (r >> 2);
        g = (g << 2) | (g >> 4);
        b = (b << 3) | (b >> 2);
        return 0xff000000u | (r << 16) | (g << 8) | b;
    }
    default:
        return 0;
    }
}

static void WritePixel32(bits_image_t* image, int x, int y, uint32_t p) {
    uint8_t* row = (uint8_t*)image->bits + image->rowstride * 4 * y;
    switch (image->format) {
    case PIXMAN_a8r8g8b8:
        ((uint32_t*)row)[x] = p;
        break;
    case PIXMAN_x8r8g8b8:
        ((uint32_t*)row)[x] = p & 0x00ffffffu;
        break;
    case PIXMAN_a8b8g8r8:
        ((uint32_t*)row)[x] = (p & 0xff000000u) | ((p & 0x000000ffu) << 16) | (p & 0x0000ff00u) | ((p & 0x00ff0000u) >> 16);
        break;
    case PIXMAN_x8b8g8r8:
        ((uint32_t*)row)[x] = ((p & 0x000000ffu) << 16) | (p & 0x0000ff00u) | ((p & 0x00ff0000u) >> 16);
        break;
    case PIXMAN_b8g8r8a8:
        ((uint32_t*)row)[x] = ((p & 0x000000ffu) << 24) | ((p & 0x0000ff00u) << 8) | ((p & 0x00ff0000u) >> 8) | ((p & 0xff000000u) >> 24);
        break;
    case PIXMAN_b8g8r8x8:
        ((uint32_t*)row)[x] = ((p & 0x000000ffu) << 24) | ((p & 0x0000ff00u) << 8) | ((p & 0x00ff0000u) >> 8);
        break;
    case PIXMAN_r8g8b8a8:
        ((uint32_t*)row)[x] = ((p & 0x00ff0000u) << 8) | ((p & 0x0000ff00u) << 8) | ((p & 0x000000ffu) << 8) | ((p & 0xff000000u) >> 24);
        break;
    case PIXMAN_r8g8b8x8:
        ((uint32_t*)row)[x] = ((p & 0x00ff0000u) << 8) | ((p & 0x0000ff00u) << 8) | ((p & 0x000000ffu) << 8);
        break;
    case PIXMAN_a8:
        row[x] = (uint8_t)(p >> 24);
        break;
    case PIXMAN_r5g6b5: {
        uint16_t r = (uint16_t)((p >> 19) & 31);
        uint16_t g = (uint16_t)((p >> 10) & 63);
        uint16_t b = (uint16_t)((p >> 3) & 31);
        ((uint16_t*)row)[x] = (uint16_t)((r << 11) | (g << 5) | b);
        break;
    }
    case PIXMAN_b5g6r5: {
        uint16_t r = (uint16_t)((p >> 19) & 31);
        uint16_t g = (uint16_t)((p >> 10) & 63);
        uint16_t b = (uint16_t)((p >> 3) & 31);
        ((uint16_t*)row)[x] = (uint16_t)((b << 11) | (g << 5) | r);
        break;
    }
    default:
        break;
    }
}

static void FetchScanline32(bits_image_t* image, int x, int y, int width, uint32_t* buffer, const uint32_t* mask) {
    int i;
    for (i = 0; i < width; ++i) {
        if (!mask || mask[i]) buffer[i] = ReadPixel32(image, x + i, y);
    }
}

static void StoreScanline32(bits_image_t* image, int x, int y, int width, const uint32_t* values) {
    int i;
    for (i = 0; i < width; ++i) WritePixel32(image, x + i, y, values[i]);
}

static argb_t FetchPixelFloat(bits_image_t* image, int x, int y) {
    uint32_t p = ReadPixel32(image, x, y);
    argb_t v;
    v.a = ((p >> 24) & 255) / 255.0f;
    v.r = ((p >> 16) & 255) / 255.0f;
    v.g = ((p >> 8) & 255) / 255.0f;
    v.b = (p & 255) / 255.0f;
    return v;
}

static void FetchScanlineFloat(bits_image_t* image, int x, int y, int width, uint32_t* buffer, const uint32_t* mask) {
    int i;
    argb_t* out = (argb_t*)buffer;
    for (i = 0; i < width; ++i) {
        if (!mask || mask[i]) out[i] = FetchPixelFloat(image, x + i, y);
    }
}

static void StoreScanlineFloat(bits_image_t* image, int x, int y, int width, const uint32_t* values) {
    int i;
    const argb_t* in = (const argb_t*)values;
    for (i = 0; i < width; ++i) {
        uint32_t a = (uint32_t)CLIP((int)(in[i].a * 255.0f + 0.5f), 0, 255);
        uint32_t r = (uint32_t)CLIP((int)(in[i].r * 255.0f + 0.5f), 0, 255);
        uint32_t g = (uint32_t)CLIP((int)(in[i].g * 255.0f + 0.5f), 0, 255);
        uint32_t b = (uint32_t)CLIP((int)(in[i].b * 255.0f + 0.5f), 0, 255);
        WritePixel32(image, x + i, y, (a << 24) | (r << 16) | (g << 8) | b);
    }
}

void _pixman_bits_image_setup_accessors(bits_image_t* image) {
    image->fetch_scanline_32 = FetchScanline32;
    image->fetch_pixel_32 = ReadPixel32;
    image->store_scanline_32 = StoreScanline32;
    image->fetch_scanline_float = FetchScanlineFloat;
    image->fetch_pixel_float = FetchPixelFloat;
    image->store_scanline_float = StoreScanlineFloat;
}

pixman_implementation_t* _pixman_implementation_create_fast_path(pixman_implementation_t* fallback) {
    return fallback;
}

void _pixman_setup_combiner_functions_float(pixman_implementation_t* imp) {
    (void)imp;
}

static void InitUnusedGradient(pixman_image_t* image, pixman_iter_t* iter) {
    (void)image;
    iter->get_scanline = _pixman_iter_get_scanline_noop;
    iter->write_back = 0;
    iter->fini = 0;
}

void _pixman_linear_gradient_iter_init(pixman_image_t* image, pixman_iter_t* iter) { InitUnusedGradient(image, iter); }
void _pixman_radial_gradient_iter_init(pixman_image_t* image, pixman_iter_t* iter) { InitUnusedGradient(image, iter); }
void _pixman_conical_gradient_iter_init(pixman_image_t* image, pixman_iter_t* iter) { InitUnusedGradient(image, iter); }

static int BoxEmpty32(const pixman_box32_t* box) {
    return box->x1 >= box->x2 || box->y1 >= box->y2;
}

static int BoxEmpty16(const pixman_box16_t* box) {
    return box->x1 >= box->x2 || box->y1 >= box->y2;
}

void pixman_region32_init(pixman_region32_t* region) {
    region->extents.x1 = region->extents.y1 = region->extents.x2 = region->extents.y2 = 0;
    region->data = 0;
}

void pixman_region32_init_rect(pixman_region32_t* region, int x, int y, unsigned int width, unsigned int height) {
    region->extents.x1 = x;
    region->extents.y1 = y;
    region->extents.x2 = x + (int)width;
    region->extents.y2 = y + (int)height;
    region->data = 0;
}

pixman_bool_t pixman_region32_init_rects(pixman_region32_t* region, const pixman_box32_t* boxes, int count) {
    int i;
    pixman_region32_init(region);
    if (count <= 0) return TRUE;
    region->extents = boxes[0];
    for (i = 1; i < count; ++i) {
        if (boxes[i].x1 < region->extents.x1) region->extents.x1 = boxes[i].x1;
        if (boxes[i].y1 < region->extents.y1) region->extents.y1 = boxes[i].y1;
        if (boxes[i].x2 > region->extents.x2) region->extents.x2 = boxes[i].x2;
        if (boxes[i].y2 > region->extents.y2) region->extents.y2 = boxes[i].y2;
    }
    return TRUE;
}

void pixman_region32_init_with_extents(pixman_region32_t* region, pixman_box32_t* extents) { region->extents = *extents; region->data = 0; }
void pixman_region32_fini(pixman_region32_t* region) { region->data = 0; }
void pixman_region32_clear(pixman_region32_t* region) { pixman_region32_init(region); }

pixman_bool_t pixman_region32_copy(pixman_region32_t* dest, pixman_region32_t* source) {
    dest->extents = source->extents;
    dest->data = 0;
    return TRUE;
}

pixman_bool_t pixman_region32_intersect(pixman_region32_t* dest, pixman_region32_t* a, pixman_region32_t* b) {
    dest->extents.x1 = MAX(a->extents.x1, b->extents.x1);
    dest->extents.y1 = MAX(a->extents.y1, b->extents.y1);
    dest->extents.x2 = MIN(a->extents.x2, b->extents.x2);
    dest->extents.y2 = MIN(a->extents.y2, b->extents.y2);
    dest->data = 0;
    if (BoxEmpty32(&dest->extents)) pixman_region32_clear(dest);
    return TRUE;
}

pixman_bool_t pixman_region32_intersect_rect(pixman_region32_t* dest, pixman_region32_t* source, int x, int y, unsigned int width, unsigned int height) {
    pixman_region32_t rect;
    pixman_region32_init_rect(&rect, x, y, width, height);
    return pixman_region32_intersect(dest, source, &rect);
}

pixman_bool_t pixman_region32_union(pixman_region32_t* dest, pixman_region32_t* a, pixman_region32_t* b) {
    if (BoxEmpty32(&a->extents)) return pixman_region32_copy(dest, b);
    if (BoxEmpty32(&b->extents)) return pixman_region32_copy(dest, a);
    dest->extents.x1 = MIN(a->extents.x1, b->extents.x1);
    dest->extents.y1 = MIN(a->extents.y1, b->extents.y1);
    dest->extents.x2 = MAX(a->extents.x2, b->extents.x2);
    dest->extents.y2 = MAX(a->extents.y2, b->extents.y2);
    dest->data = 0;
    return TRUE;
}

pixman_bool_t pixman_region32_union_rect(pixman_region32_t* dest, pixman_region32_t* source, int x, int y, unsigned int width, unsigned int height) {
    pixman_region32_t rect;
    pixman_region32_init_rect(&rect, x, y, width, height);
    return pixman_region32_union(dest, source, &rect);
}

void pixman_region32_translate(pixman_region32_t* region, int x, int y) {
    region->extents.x1 += x; region->extents.x2 += x;
    region->extents.y1 += y; region->extents.y2 += y;
}

pixman_bool_t pixman_region32_not_empty(pixman_region32_t* region) { return !BoxEmpty32(&region->extents); }
pixman_box32_t* pixman_region32_extents(pixman_region32_t* region) { return &region->extents; }
int pixman_region32_n_rects(pixman_region32_t* region) { return BoxEmpty32(&region->extents) ? 0 : 1; }
pixman_box32_t* pixman_region32_rectangles(pixman_region32_t* region, int* count) { if (count) *count = pixman_region32_n_rects(region); return &region->extents; }
pixman_bool_t pixman_region32_equal(pixman_region32_t* a, pixman_region32_t* b) { return memcmp(&a->extents, &b->extents, sizeof(a->extents)) == 0; }
void pixman_region32_reset(pixman_region32_t* region, pixman_box32_t* box) { region->extents = *box; region->data = 0; }
pixman_bool_t pixman_region32_selfcheck(pixman_region32_t* region) { (void)region; return TRUE; }

pixman_bool_t pixman_region32_contains_point(pixman_region32_t* region, int x, int y, pixman_box32_t* box) {
    pixman_bool_t inside = x >= region->extents.x1 && x < region->extents.x2 && y >= region->extents.y1 && y < region->extents.y2;
    if (inside && box) *box = region->extents;
    return inside;
}

pixman_region_overlap_t pixman_region32_contains_rectangle(pixman_region32_t* region, pixman_box32_t* rect) {
    if (rect->x2 <= region->extents.x1 || rect->x1 >= region->extents.x2 || rect->y2 <= region->extents.y1 || rect->y1 >= region->extents.y2) return PIXMAN_REGION_OUT;
    if (rect->x1 >= region->extents.x1 && rect->x2 <= region->extents.x2 && rect->y1 >= region->extents.y1 && rect->y2 <= region->extents.y2) return PIXMAN_REGION_IN;
    return PIXMAN_REGION_PART;
}

void pixman_region_init(pixman_region16_t* region) {
    region->extents.x1 = region->extents.y1 = region->extents.x2 = region->extents.y2 = 0;
    region->data = 0;
}

void pixman_region_init_rect(pixman_region16_t* region, int x, int y, unsigned int width, unsigned int height) {
    region->extents.x1 = (int16_t)x; region->extents.y1 = (int16_t)y;
    region->extents.x2 = (int16_t)(x + width); region->extents.y2 = (int16_t)(y + height);
    region->data = 0;
}

pixman_bool_t pixman_region_init_rects(pixman_region16_t* region, const pixman_box16_t* boxes, int count) {
    int i;
    pixman_region_init(region);
    if (count <= 0) return TRUE;
    region->extents = boxes[0];
    for (i = 1; i < count; ++i) {
        if (boxes[i].x1 < region->extents.x1) region->extents.x1 = boxes[i].x1;
        if (boxes[i].y1 < region->extents.y1) region->extents.y1 = boxes[i].y1;
        if (boxes[i].x2 > region->extents.x2) region->extents.x2 = boxes[i].x2;
        if (boxes[i].y2 > region->extents.y2) region->extents.y2 = boxes[i].y2;
    }
    return TRUE;
}

void pixman_region_init_with_extents(pixman_region16_t* region, pixman_box16_t* extents) { region->extents = *extents; region->data = 0; }
void pixman_region_fini(pixman_region16_t* region) { region->data = 0; }
void pixman_region_clear(pixman_region16_t* region) { pixman_region_init(region); }

pixman_bool_t pixman_region_copy(pixman_region16_t* dest, pixman_region16_t* source) { dest->extents = source->extents; dest->data = 0; return TRUE; }

pixman_bool_t pixman_region_intersect(pixman_region16_t* dest, pixman_region16_t* a, pixman_region16_t* b) {
    dest->extents.x1 = MAX(a->extents.x1, b->extents.x1); dest->extents.y1 = MAX(a->extents.y1, b->extents.y1);
    dest->extents.x2 = MIN(a->extents.x2, b->extents.x2); dest->extents.y2 = MIN(a->extents.y2, b->extents.y2);
    dest->data = 0;
    if (BoxEmpty16(&dest->extents)) pixman_region_clear(dest);
    return TRUE;
}

pixman_bool_t pixman_region_intersect_rect(pixman_region16_t* dest, pixman_region16_t* source, int x, int y, unsigned int width, unsigned int height) {
    pixman_region16_t rect;
    pixman_region_init_rect(&rect, x, y, width, height);
    return pixman_region_intersect(dest, source, &rect);
}

pixman_bool_t pixman_region_union(pixman_region16_t* dest, pixman_region16_t* a, pixman_region16_t* b) {
    if (BoxEmpty16(&a->extents)) return pixman_region_copy(dest, b);
    if (BoxEmpty16(&b->extents)) return pixman_region_copy(dest, a);
    dest->extents.x1 = MIN(a->extents.x1, b->extents.x1); dest->extents.y1 = MIN(a->extents.y1, b->extents.y1);
    dest->extents.x2 = MAX(a->extents.x2, b->extents.x2); dest->extents.y2 = MAX(a->extents.y2, b->extents.y2);
    dest->data = 0;
    return TRUE;
}

pixman_bool_t pixman_region_union_rect(pixman_region16_t* dest, pixman_region16_t* source, int x, int y, unsigned int width, unsigned int height) {
    pixman_region16_t rect;
    pixman_region_init_rect(&rect, x, y, width, height);
    return pixman_region_union(dest, source, &rect);
}

void pixman_region_translate(pixman_region16_t* region, int x, int y) { region->extents.x1 += x; region->extents.x2 += x; region->extents.y1 += y; region->extents.y2 += y; }
pixman_bool_t pixman_region_not_empty(pixman_region16_t* region) { return !BoxEmpty16(&region->extents); }
pixman_box16_t* pixman_region_extents(pixman_region16_t* region) { return &region->extents; }
int pixman_region_n_rects(pixman_region16_t* region) { return BoxEmpty16(&region->extents) ? 0 : 1; }
pixman_box16_t* pixman_region_rectangles(pixman_region16_t* region, int* count) { if (count) *count = pixman_region_n_rects(region); return &region->extents; }
pixman_bool_t pixman_region_equal(pixman_region16_t* a, pixman_region16_t* b) { return memcmp(&a->extents, &b->extents, sizeof(a->extents)) == 0; }
void pixman_region_reset(pixman_region16_t* region, pixman_box16_t* box) { region->extents = *box; region->data = 0; }
pixman_bool_t pixman_region_selfcheck(pixman_region16_t* region) { (void)region; return TRUE; }

pixman_bool_t pixman_region_contains_point(pixman_region16_t* region, int x, int y, pixman_box16_t* box) {
    pixman_bool_t inside = x >= region->extents.x1 && x < region->extents.x2 && y >= region->extents.y1 && y < region->extents.y2;
    if (inside && box) *box = region->extents;
    return inside;
}

pixman_region_overlap_t pixman_region_contains_rectangle(pixman_region16_t* region, pixman_box16_t* rect) {
    if (rect->x2 <= region->extents.x1 || rect->x1 >= region->extents.x2 || rect->y2 <= region->extents.y1 || rect->y1 >= region->extents.y2) return PIXMAN_REGION_OUT;
    if (rect->x1 >= region->extents.x1 && rect->x2 <= region->extents.x2 && rect->y1 >= region->extents.y1 && rect->y2 <= region->extents.y2) return PIXMAN_REGION_IN;
    return PIXMAN_REGION_PART;
}

void pixman_region_set_static_pointers(pixman_box16_t* a, pixman_region16_data_t* b, pixman_region16_data_t* c) { (void)a; (void)b; (void)c; }

pixman_bool_t _pixman_compute_composite_region32(pixman_region32_t* region, pixman_image_t* src, pixman_image_t* mask, pixman_image_t* dest, int32_t sx, int32_t sy, int32_t mx, int32_t my, int32_t dx, int32_t dy, int32_t width, int32_t height) {
    (void)src; (void)mask; (void)sx; (void)sy; (void)mx; (void)my;
    pixman_region32_init_rect(region, dx, dy, width, height);
    return pixman_region32_intersect_rect(region, region, 0, 0, dest->bits.width, dest->bits.height);
}

pixman_bool_t pixman_compute_composite_region(pixman_region16_t* region, pixman_image_t* src, pixman_image_t* mask, pixman_image_t* dest, int16_t sx, int16_t sy, int16_t mx, int16_t my, int16_t dx, int16_t dy, uint16_t width, uint16_t height) {
    pixman_region32_t r32;
    pixman_bool_t ok = _pixman_compute_composite_region32(&r32, src, mask, dest, sx, sy, mx, my, dx, dy, width, height);
    pixman_region_init_rect(region, r32.extents.x1, r32.extents.y1, r32.extents.x2 - r32.extents.x1, r32.extents.y2 - r32.extents.y1);
    return ok;
}

void pixman_image_composite32(pixman_op_t op, pixman_image_t* src, pixman_image_t* mask, pixman_image_t* dest, int32_t sx, int32_t sy, int32_t mx, int32_t my, int32_t dx, int32_t dy, int32_t width, int32_t height) {
    pixman_composite_info_t info;
    pixman_implementation_t* imp;
    pixman_composite_func_t func;
    pixman_region32_t region;
    if (!src || !dest || width <= 0 || height <= 0) return;
    _pixman_image_validate(src);
    if (mask) _pixman_image_validate(mask);
    _pixman_image_validate(dest);
    if (!_pixman_compute_composite_region32(&region, src, mask, dest, sx, sy, mx, my, dx, dy, width, height)) return;
    if (!pixman_region32_not_empty(&region)) return;
    info.op = op;
    info.src_image = src;
    info.mask_image = mask;
    info.dest_image = dest;
    info.src_x = sx + region.extents.x1 - dx;
    info.src_y = sy + region.extents.y1 - dy;
    info.mask_x = mx + region.extents.x1 - dx;
    info.mask_y = my + region.extents.y1 - dy;
    info.dest_x = region.extents.x1;
    info.dest_y = region.extents.y1;
    info.width = region.extents.x2 - region.extents.x1;
    info.height = region.extents.y2 - region.extents.y1;
    info.src_flags = src->common.flags;
    info.mask_flags = mask ? mask->common.flags : 0;
    info.dest_flags = dest->common.flags;
    imp = get_implementation();
    _pixman_implementation_lookup_composite(imp, op, src->common.extended_format_code, info.src_flags, mask ? mask->common.extended_format_code : PIXMAN_null, info.mask_flags, dest->common.extended_format_code, info.dest_flags, &imp, &func);
    func(imp, &info);
}

void pixman_image_composite(pixman_op_t op, pixman_image_t* src, pixman_image_t* mask, pixman_image_t* dest, int16_t sx, int16_t sy, int16_t mx, int16_t my, int16_t dx, int16_t dy, uint16_t width, uint16_t height) {
    pixman_image_composite32(op, src, mask, dest, sx, sy, mx, my, dx, dy, width, height);
}

pixman_bool_t pixman_image_fill_rectangles(pixman_op_t op, pixman_image_t* image, const pixman_color_t* color, int count, const pixman_rectangle16_t* rects) {
    int i;
    pixman_image_t* solid = pixman_image_create_solid_fill(color);
    if (!solid) return FALSE;
    for (i = 0; i < count; ++i) pixman_image_composite32(op, solid, 0, image, 0, 0, 0, 0, rects[i].x, rects[i].y, rects[i].width, rects[i].height);
    pixman_image_unref(solid);
    return TRUE;
}

pixman_bool_t pixman_image_fill_boxes(pixman_op_t op, pixman_image_t* image, const pixman_color_t* color, int count, const pixman_box32_t* boxes) {
    int i;
    pixman_image_t* solid = pixman_image_create_solid_fill(color);
    if (!solid) return FALSE;
    for (i = 0; i < count; ++i) pixman_image_composite32(op, solid, 0, image, 0, 0, 0, 0, boxes[i].x1, boxes[i].y1, boxes[i].x2 - boxes[i].x1, boxes[i].y2 - boxes[i].y1);
    pixman_image_unref(solid);
    return TRUE;
}

pixman_bool_t pixman_fill(uint32_t* bits, int stride, int bpp, int x, int y, int width, int height, uint32_t value) {
    int row;
    if (bpp != 32) return FALSE;
    for (row = 0; row < height; ++row) {
        int i;
        uint32_t* dst = bits + (y + row) * stride + x;
        for (i = 0; i < width; ++i) dst[i] = value;
    }
    return TRUE;
}

pixman_bool_t pixman_blt(uint32_t* src, uint32_t* dst, int src_stride, int dst_stride, int src_bpp, int dst_bpp, int sx, int sy, int dx, int dy, int width, int height) {
    int row;
    if (src_bpp != dst_bpp || src_bpp != 32) return FALSE;
    for (row = 0; row < height; ++row) memmove(dst + (dy + row) * dst_stride + dx, src + (sy + row) * src_stride + sx, width * 4);
    return TRUE;
}

pixman_bool_t pixman_format_supported_destination(pixman_format_code_t format) {
    return format == PIXMAN_a8r8g8b8 ||
           format == PIXMAN_x8r8g8b8 ||
           format == PIXMAN_a8b8g8r8 ||
           format == PIXMAN_x8b8g8r8 ||
           format == PIXMAN_b8g8r8a8 ||
           format == PIXMAN_b8g8r8x8 ||
           format == PIXMAN_r8g8b8a8 ||
           format == PIXMAN_r8g8b8x8 ||
           format == PIXMAN_a8 ||
           format == PIXMAN_r5g6b5 ||
           format == PIXMAN_b5g6r5;
}

pixman_bool_t pixman_format_supported_source(pixman_format_code_t format) { return pixman_format_supported_destination(format); }
int pixman_version(void) { return PIXMAN_VERSION; }
const char* pixman_version_string(void) { return PIXMAN_VERSION_STRING; }

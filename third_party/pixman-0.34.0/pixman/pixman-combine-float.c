/*
 * Copyright 2010, 2011, Oracle and/or its affiliates. All rights reserved.
 * Copyright 2011 Red Hat, Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <math.h>
#include <string.h>
#include "pixman-private.h"

#define MAKE_PD_COMBINERS(name, a, b)                                  \
static void                                                            \
combine_ ## name ## _ca_float (pixman_implementation_t *imp,           \
                               pixman_op_t              op,            \
                               float *                  dest,          \
                               const float *            src,           \
                               const float *            mask,          \
                               int                      n_pixels)      \
{                                                                       \
    int i;                                                              \
                                                                        \
    for (i = 0; i < n_pixels; ++i)                                     \
    {                                                                   \
        float m;                                                        \
        float sa;                                                       \
        float da;                                                       \
        float sda;                                                      \
        float isa;                                                      \
        float ida;                                                      \
        float f;                                                        \
        float fa;                                                       \
        float fb;                                                       \
        float *s = (float *)(src + i * 4);                              \
        float *d = dest + i * 4;                                       \
        int j;                                                          \
                                                                        \
        m = *(mask + i * 4 + 0);                                       \
        sa = s[0] * m;                                                  \
        da = d[0];                                                      \
        sda = m;                                                        \
        isa = 1.0f - sa;                                                \
        ida = 1.0f - da;                                                \
        f = fa = fb = 0.0f;                                            \
        a;                                                              \
        b;                                                              \
        d[0] = CLIP (sa * fa + da * fb, 0.0f, 1.0f);                   \
                                                                        \
        for (j = 1; j < 4; j++)                                        \
        {                                                               \
            m = *(mask + i * 4 + j);                                   \
            sda = s[j] * m;                                            \
            sa = s[0] * m;                                              \
            isa = 1.0f - sa;                                            \
            fa = fb = 0.0f;                                            \
            a;                                                          \
            b;                                                          \
            d[j] = CLIP (sda * fa + d[j] * fb, 0.0f, 1.0f);            \
        }                                                               \
    }                                                                   \
}                                                                       \
                                                                        \
static void                                                             \
combine_ ## name ## _float (pixman_implementation_t *imp,              \
                            pixman_op_t              op,                \
                            float *                  dest,              \
                            const float *            src,               \
                            const float *            mask,              \
                            int                      n_pixels)          \
{                                                                       \
    int i;                                                              \
                                                                        \
    for (i = 0; i < n_pixels; ++i)                                     \
    {                                                                   \
        float *s = (float *)(src + i * 4);                              \
        float *d = dest + i * 4;                                       \
        float sa = s[0];                                                \
        float da = d[0];                                                \
        float isa = 1.0f - sa;                                          \
        float ida = 1.0f - da;                                          \
        float fa = 0.0f, fb = 0.0f;                                    \
        int j;                                                          \
                                                                        \
        a;                                                              \
        b;                                                              \
                                                                        \
        for (j = 0; j < 4; j++)                                        \
            d[j] = CLIP (s[j] * fa + d[j] * fb, 0.0f, 1.0f);           \
    }                                                                   \
}

#define MAKE_SEPARABLE_PDF_COMBINERS(name, fa, fb)                     \
    MAKE_PD_COMBINERS (name, fa, fb)

#define MAKE_UNIFIED_PDF_COMBINERS(name, f)                             \
    MAKE_PD_COMBINERS (name, f; fa = fb = f, )

MAKE_UNIFIED_PDF_COMBINERS (clear,
                            f = 0.0f)
MAKE_UNIFIED_PDF_COMBINERS (src,
                            f = 1.0f)
MAKE_UNIFIED_PDF_COMBINERS (dst,
                            f = 0.0f)
MAKE_SEPARABLE_PDF_COMBINERS (over,
                              fa = 1.0f,
                              fb = isa)
MAKE_SEPARABLE_PDF_COMBINERS (over_reverse,
                              fa = ida,
                              fb = 1.0f)
MAKE_UNIFIED_PDF_COMBINERS (in,
                            f = da)
MAKE_UNIFIED_PDF_COMBINERS (in_reverse,
                            f = sa)
MAKE_UNIFIED_PDF_COMBINERS (out,
                            f = ida)
MAKE_UNIFIED_PDF_COMBINERS (out_reverse,
                            f = isa)
MAKE_SEPARABLE_PDF_COMBINERS (atop,
                              fa = da,
                              fb = isa)
MAKE_SEPARABLE_PDF_COMBINERS (atop_reverse,
                              fa = ida,
                              fb = sa)
MAKE_SEPARABLE_PDF_COMBINERS (xor,
                              fa = ida,
                              fb = isa)
MAKE_UNIFIED_PDF_COMBINERS (add,
                            f = 1.0f)
MAKE_SEPARABLE_PDF_COMBINERS (saturate,
                              fa = sa > ida ? ida / sa : 1.0f,
                              fb = 1.0f)

MAKE_SEPARABLE_PDF_COMBINERS (disjoint_over,
                              fa = 1.0f,
                              fb = da == 0.0f ? 1.0f : MIN (1.0f, isa / da))
MAKE_SEPARABLE_PDF_COMBINERS (disjoint_over_reverse,
                              fa = sa == 0.0f ? 1.0f : MIN (1.0f, ida / sa),
                              fb = 1.0f)
MAKE_UNIFIED_PDF_COMBINERS (disjoint_in,
                            f = sa == 0.0f ? 0.0f : MAX (1.0f - (1.0f - da) / sa, 0.0f))
MAKE_UNIFIED_PDF_COMBINERS (disjoint_in_reverse,
                            f = da == 0.0f ? 0.0f : MAX (1.0f - (1.0f - sa) / da, 0.0f))
MAKE_UNIFIED_PDF_COMBINERS (disjoint_out,
                            f = sa == 0.0f ? 1.0f : MIN ((1.0f - da) / sa, 1.0f))
MAKE_UNIFIED_PDF_COMBINERS (disjoint_out_reverse,
                            f = da == 0.0f ? 1.0f : MIN ((1.0f - sa) / da, 1.0f))
MAKE_SEPARABLE_PDF_COMBINERS (disjoint_atop,
                              fa = sa == 0.0f ? 0.0f : MAX (1.0f - (1.0f - da) / sa, 0.0f),
                              fb = da == 0.0f ? 1.0f : MIN ((1.0f - sa) / da, 1.0f))
MAKE_SEPARABLE_PDF_COMBINERS (disjoint_atop_reverse,
                              fa = sa == 0.0f ? 1.0f : MIN ((1.0f - da) / sa, 1.0f),
                              fb = da == 0.0f ? 0.0f : MAX (1.0f - (1.0f - sa) / da, 0.0f))
MAKE_SEPARABLE_PDF_COMBINERS (disjoint_xor,
                              fa = sa == 0.0f ? 1.0f : MIN ((1.0f - da) / sa, 1.0f),
                              fb = da == 0.0f ? 1.0f : MIN ((1.0f - sa) / da, 1.0f))

MAKE_SEPARABLE_PDF_COMBINERS (conjoint_over,
                              fa = 1.0f,
                              fb = da == 0.0f ? 0.0f : MAX (1.0f - sa / da, 0.0f))
MAKE_SEPARABLE_PDF_COMBINERS (conjoint_over_reverse,
                              fa = sa == 0.0f ? 0.0f : MAX (1.0f - da / sa, 0.0f),
                              fb = 1.0f)
MAKE_UNIFIED_PDF_COMBINERS (conjoint_in,
                            f = sa == 0.0f ? 0.0f : MIN (da / sa, 1.0f))
MAKE_UNIFIED_PDF_COMBINERS (conjoint_in_reverse,
                            f = da == 0.0f ? 0.0f : MIN (sa / da, 1.0f))
MAKE_UNIFIED_PDF_COMBINERS (conjoint_out,
                            f = sa == 0.0f ? 0.0f : MAX (1.0f - da / sa, 0.0f))
MAKE_UNIFIED_PDF_COMBINERS (conjoint_out_reverse,
                            f = da == 0.0f ? 0.0f : MAX (1.0f - sa / da, 0.0f))
MAKE_SEPARABLE_PDF_COMBINERS (conjoint_atop,
                              fa = sa == 0.0f ? 0.0f : MIN (da / sa, 1.0f),
                              fb = da == 0.0f ? 0.0f : MAX (1.0f - sa / da, 0.0f))
MAKE_SEPARABLE_PDF_COMBINERS (conjoint_atop_reverse,
                              fa = sa == 0.0f ? 0.0f : MAX (1.0f - da / sa, 0.0f),
                              fb = da == 0.0f ? 0.0f : MIN (sa / da, 1.0f))
MAKE_SEPARABLE_PDF_COMBINERS (conjoint_xor,
                              fa = sa == 0.0f ? 0.0f : MAX (1.0f - da / sa, 0.0f),
                              fb = da == 0.0f ? 0.0f : MAX (1.0f - sa / da, 0.0f))

static force_inline float
blend_multiply (float sa, float s, float da, float d)
{
    return d * s;
}

static force_inline float
blend_screen (float sa, float s, float da, float d)
{
    return d * sa + s * da - s * d;
}

static force_inline float
blend_overlay (float sa, float s, float da, float d)
{
    if (2 * d < da)
        return 2 * s * d;
    else
        return sa * da - 2 * (da - d) * (sa - s);
}

static force_inline float
blend_darken (float sa, float s, float da, float d)
{
    return MIN (s * da, d * sa);
}

static force_inline float
blend_lighten (float sa, float s, float da, float d)
{
    return MAX (s * da, d * sa);
}

static force_inline float
blend_color_dodge (float sa, float s, float da, float d)
{
    if (d == 0.0f)
        return 0.0f;
    else if (d * sa >= sa * (da - s))
        return sa * da;
    else
        return d * sa / (sa - s);
}

static force_inline float
blend_color_burn (float sa, float s, float da, float d)
{
    if (d >= da)
        return sa * da;
    else if (sa * (da - d) >= s * da)
        return 0.0f;
    else
        return sa * (da - (da - d) * sa / s);
}

static force_inline float
blend_hard_light (float sa, float s, float da, float d)
{
    if (2 * s < sa)
        return 2 * s * d;
    else
        return sa * da - 2 * (da - d) * (sa - s);
}

static force_inline float
blend_soft_light (float sa, float s, float da, float d)
{
    if (2 * s <= sa)
    {
        if (da == 0.0f)
            return d * sa;
        else
            return d * sa - d * (da - d) * (sa - 2 * s) / da;
    }
    else
    {
        if (da == 0.0f)
        {
            return 0.0f;
        }
        else
        {
            float m = d / da;
            float g;

            if (4 * d <= da)
                g = ((16 * m - 12) * m + 4) * m;
            else
                g = sqrtf (m);

            return d * sa + (g * da - d) * (2 * s - sa);
        }
    }
}

static force_inline float
blend_difference (float sa, float s, float da, float d)
{
    return d * sa + s * da - 2 * MIN (s * da, d * sa);
}

static force_inline float
blend_exclusion (float sa, float s, float da, float d)
{
    return d * sa + s * da - 2 * d * s;
}

#define MAKE_SEPARABLE_BLEND_MODE(name)                                 \
static void                                                             \
combine_ ## name ## _float (pixman_implementation_t *imp,               \
                            pixman_op_t              op,                \
                            float *                  dest,              \
                            const float *            src,               \
                            const float *            mask,              \
                            int                      n_pixels)           \
{                                                                        \
    int i;                                                              \
                                                                         \
    for (i = 0; i < n_pixels; ++i)                                      \
    {                                                                    \
        float *s = (float *)(src + i * 4);                               \
        float *d = dest + i * 4;                                        \
        float sa = s[0];                                                 \
        float da = d[0];                                                 \
        float isa = 1.0f - sa;                                           \
        float ida = 1.0f - da;                                           \
        int j;                                                           \
                                                                         \
        d[0] = sa + da - sa * da;                                        \
                                                                         \
        for (j = 1; j < 4; j++)                                         \
        {                                                                \
            float f = blend_ ## name (sa, s[j], da, d[j]);               \
            d[j] = CLIP (f + s[j] * ida + d[j] * isa, 0.0f, 1.0f);      \
        }                                                                \
    }                                                                    \
}                                                                        \
                                                                         \
static void                                                              \
combine_ ## name ## _ca_float (pixman_implementation_t *imp,            \
                               pixman_op_t              op,              \
                               float *                  dest,            \
                               const float *            src,             \
                               const float *            mask,            \
                               int                      n_pixels)        \
{                                                                        \
    int i;                                                              \
                                                                         \
    for (i = 0; i < n_pixels; ++i)                                      \
    {                                                                    \
        float *s = (float *)(src + i * 4);                               \
        float *d = dest + i * 4;                                        \
        const float *m = mask + i * 4;                                   \
        float sa = s[0] * m[0];                                          \
        float da = d[0];                                                 \
        float isa = 1.0f - sa;                                           \
        float ida = 1.0f - da;                                           \
        int j;                                                           \
                                                                         \
        d[0] = sa + da - sa * da;                                        \
                                                                         \
        for (j = 1; j < 4; j++)                                         \
        {                                                                \
            float sda = s[j] * m[j];                                     \
            float f = blend_ ## name (sa, sda, da, d[j]);                \
            d[j] = CLIP (f + sda * ida + d[j] * isa, 0.0f, 1.0f);       \
        }                                                                \
    }                                                                    \
}

MAKE_SEPARABLE_BLEND_MODE (multiply)
MAKE_SEPARABLE_BLEND_MODE (screen)
MAKE_SEPARABLE_BLEND_MODE (overlay)
MAKE_SEPARABLE_BLEND_MODE (darken)
MAKE_SEPARABLE_BLEND_MODE (lighten)
MAKE_SEPARABLE_BLEND_MODE (color_dodge)
MAKE_SEPARABLE_BLEND_MODE (color_burn)
MAKE_SEPARABLE_BLEND_MODE (hard_light)
MAKE_SEPARABLE_BLEND_MODE (soft_light)
MAKE_SEPARABLE_BLEND_MODE (difference)
MAKE_SEPARABLE_BLEND_MODE (exclusion)

static force_inline float
minf (float a, float b)
{
    return a < b ? a : b;
}

static force_inline float
maxf (float a, float b)
{
    return a > b ? a : b;
}

static float
get_lum (const float *c)
{
    return 0.3f * c[1] + 0.59f * c[2] + 0.11f * c[3];
}

static void
set_lum (float *dest, float sa, float lum)
{
    float d = lum - get_lum (dest);
    float l;
    float n;
    float x;

    dest[1] += d;
    dest[2] += d;
    dest[3] += d;

    l = get_lum (dest);
    n = minf (minf (dest[1], dest[2]), dest[3]);
    x = maxf (maxf (dest[1], dest[2]), dest[3]);

    if (n < 0.0f)
    {
        dest[1] = l + ((dest[1] - l) * l) / (l - n);
        dest[2] = l + ((dest[2] - l) * l) / (l - n);
        dest[3] = l + ((dest[3] - l) * l) / (l - n);
    }

    if (x > sa)
    {
        dest[1] = l + ((dest[1] - l) * (sa - l)) / (x - l);
        dest[2] = l + ((dest[2] - l) * (sa - l)) / (x - l);
        dest[3] = l + ((dest[3] - l) * (sa - l)) / (x - l);
    }
}

static float
get_sat (const float *c)
{
    return maxf (maxf (c[1], c[2]), c[3]) - minf (minf (c[1], c[2]), c[3]);
}

static void
set_sat (float *dest, float sat)
{
    int id[3];
    float *c[3];
    int i;

    c[0] = &dest[1];
    c[1] = &dest[2];
    c[2] = &dest[3];

    for (i = 0; i < 3; ++i)
        id[i] = i;

    if (*c[id[0]] > *c[id[1]])
    {
        int tmp = id[0];
        id[0] = id[1];
        id[1] = tmp;
    }

    if (*c[id[1]] > *c[id[2]])
    {
        int tmp = id[1];
        id[1] = id[2];
        id[2] = tmp;
    }

    if (*c[id[0]] > *c[id[1]])
    {
        int tmp = id[0];
        id[0] = id[1];
        id[1] = tmp;
    }

    if (*c[id[2]] > *c[id[0]])
    {
        *c[id[1]] = ((*c[id[1]] - *c[id[0]]) * sat) /
                    (*c[id[2]] - *c[id[0]]);
        *c[id[2]] = sat;
    }
    else
    {
        *c[id[1]] = 0.0f;
        *c[id[2]] = 0.0f;
    }

    *c[id[0]] = 0.0f;
}

#define MAKE_NON_SEPARABLE_BLEND_MODE(name, body)                       \
static void                                                             \
combine_ ## name ## _float (pixman_implementation_t *imp,               \
                            pixman_op_t              op,                \
                            float *                  dest,              \
                            const float *            src,               \
                            const float *            mask,              \
                            int                      n_pixels)           \
{                                                                        \
    int i;                                                              \
                                                                         \
    for (i = 0; i < n_pixels; ++i)                                      \
    {                                                                    \
        float *s = (float *)(src + i * 4);                               \
        float *d = dest + i * 4;                                        \
        float sa = s[0];                                                 \
        float da = d[0];                                                 \
        float result[4];                                                 \
        int j;                                                           \
                                                                         \
        result[0] = sa + da - sa * da;                                   \
        body;                                                            \
                                                                         \
        for (j = 1; j < 4; j++)                                         \
        {                                                                \
            result[j] = (1.0f - sa) * d[j] + (1.0f - da) * s[j] +       \
                        result[j];                                       \
        }                                                                \
                                                                         \
        memcpy (d, result, sizeof (result));                             \
    }                                                                    \
}                                                                        \
                                                                         \
static void                                                              \
combine_ ## name ## _ca_float (pixman_implementation_t *imp,            \
                               pixman_op_t              op,              \
                               float *                  dest,            \
                               const float *            src,             \
                               const float *            mask,            \
                               int                      n_pixels)        \
{                                                                        \
    int i;                                                              \
                                                                         \
    for (i = 0; i < n_pixels; ++i)                                      \
    {                                                                    \
        float *s = (float *)(src + i * 4);                               \
        float *d = dest + i * 4;                                        \
        const float *m = mask + i * 4;                                   \
        float sm[4];                                                     \
        float result[4];                                                 \
        float sa;                                                        \
        float da = d[0];                                                 \
        int j;                                                           \
                                                                         \
        for (j = 0; j < 4; j++)                                         \
            sm[j] = s[j] * m[j];                                        \
                                                                         \
        sa = sm[0];                                                      \
        result[0] = sa + da - sa * da;                                   \
        s = sm;                                                          \
        body;                                                            \
                                                                         \
        for (j = 1; j < 4; j++)                                         \
        {                                                                \
            result[j] = (1.0f - sa) * d[j] + (1.0f - da) * s[j] +       \
                        result[j];                                       \
        }                                                                \
                                                                         \
        memcpy (d, result, sizeof (result));                             \
    }                                                                    \
}

MAKE_NON_SEPARABLE_BLEND_MODE (hsl_hue,
{
    float t[4] = { result[0], s[1] * da, s[2] * da, s[3] * da };
    set_sat (t, get_sat (d) * sa);
    set_lum (t, result[0], get_lum (d) * sa);
    result[1] = t[1];
    result[2] = t[2];
    result[3] = t[3];
})

MAKE_NON_SEPARABLE_BLEND_MODE (hsl_saturation,
{
    float t[4] = { result[0], d[1] * sa, d[2] * sa, d[3] * sa };
    set_sat (t, get_sat (s) * da);
    set_lum (t, result[0], get_lum (d) * sa);
    result[1] = t[1];
    result[2] = t[2];
    result[3] = t[3];
})

MAKE_NON_SEPARABLE_BLEND_MODE (hsl_color,
{
    float t[4] = { result[0], s[1] * da, s[2] * da, s[3] * da };
    set_lum (t, result[0], get_lum (d) * sa);
    result[1] = t[1];
    result[2] = t[2];
    result[3] = t[3];
})

MAKE_NON_SEPARABLE_BLEND_MODE (hsl_luminosity,
{
    float t[4] = { result[0], d[1] * sa, d[2] * sa, d[3] * sa };
    set_lum (t, result[0], get_lum (s) * da);
    result[1] = t[1];
    result[2] = t[2];
    result[3] = t[3];
})

void
_pixman_setup_combiner_functions_float (pixman_implementation_t *imp)
{
    imp->combine_float[PIXMAN_OP_CLEAR] = combine_clear_float;
    imp->combine_float[PIXMAN_OP_SRC] = combine_src_float;
    imp->combine_float[PIXMAN_OP_DST] = combine_dst_float;
    imp->combine_float[PIXMAN_OP_OVER] = combine_over_float;
    imp->combine_float[PIXMAN_OP_OVER_REVERSE] = combine_over_reverse_float;
    imp->combine_float[PIXMAN_OP_IN] = combine_in_float;
    imp->combine_float[PIXMAN_OP_IN_REVERSE] = combine_in_reverse_float;
    imp->combine_float[PIXMAN_OP_OUT] = combine_out_float;
    imp->combine_float[PIXMAN_OP_OUT_REVERSE] = combine_out_reverse_float;
    imp->combine_float[PIXMAN_OP_ATOP] = combine_atop_float;
    imp->combine_float[PIXMAN_OP_ATOP_REVERSE] = combine_atop_reverse_float;
    imp->combine_float[PIXMAN_OP_XOR] = combine_xor_float;
    imp->combine_float[PIXMAN_OP_ADD] = combine_add_float;
    imp->combine_float[PIXMAN_OP_SATURATE] = combine_saturate_float;

    imp->combine_float[PIXMAN_OP_DISJOINT_OVER] = combine_disjoint_over_float;
    imp->combine_float[PIXMAN_OP_DISJOINT_OVER_REVERSE] = combine_disjoint_over_reverse_float;
    imp->combine_float[PIXMAN_OP_DISJOINT_IN] = combine_disjoint_in_float;
    imp->combine_float[PIXMAN_OP_DISJOINT_IN_REVERSE] = combine_disjoint_in_reverse_float;
    imp->combine_float[PIXMAN_OP_DISJOINT_OUT] = combine_disjoint_out_float;
    imp->combine_float[PIXMAN_OP_DISJOINT_OUT_REVERSE] = combine_disjoint_out_reverse_float;
    imp->combine_float[PIXMAN_OP_DISJOINT_ATOP] = combine_disjoint_atop_float;
    imp->combine_float[PIXMAN_OP_DISJOINT_ATOP_REVERSE] = combine_disjoint_atop_reverse_float;
    imp->combine_float[PIXMAN_OP_DISJOINT_XOR] = combine_disjoint_xor_float;

    imp->combine_float[PIXMAN_OP_CONJOINT_OVER] = combine_conjoint_over_float;
    imp->combine_float[PIXMAN_OP_CONJOINT_OVER_REVERSE] = combine_conjoint_over_reverse_float;
    imp->combine_float[PIXMAN_OP_CONJOINT_IN] = combine_conjoint_in_float;
    imp->combine_float[PIXMAN_OP_CONJOINT_IN_REVERSE] = combine_conjoint_in_reverse_float;
    imp->combine_float[PIXMAN_OP_CONJOINT_OUT] = combine_conjoint_out_float;
    imp->combine_float[PIXMAN_OP_CONJOINT_OUT_REVERSE] = combine_conjoint_out_reverse_float;
    imp->combine_float[PIXMAN_OP_CONJOINT_ATOP] = combine_conjoint_atop_float;
    imp->combine_float[PIXMAN_OP_CONJOINT_ATOP_REVERSE] = combine_conjoint_atop_reverse_float;
    imp->combine_float[PIXMAN_OP_CONJOINT_XOR] = combine_conjoint_xor_float;

    imp->combine_float[PIXMAN_OP_MULTIPLY] = combine_multiply_float;
    imp->combine_float[PIXMAN_OP_SCREEN] = combine_screen_float;
    imp->combine_float[PIXMAN_OP_OVERLAY] = combine_overlay_float;
    imp->combine_float[PIXMAN_OP_DARKEN] = combine_darken_float;
    imp->combine_float[PIXMAN_OP_LIGHTEN] = combine_lighten_float;
    imp->combine_float[PIXMAN_OP_COLOR_DODGE] = combine_color_dodge_float;
    imp->combine_float[PIXMAN_OP_COLOR_BURN] = combine_color_burn_float;
    imp->combine_float[PIXMAN_OP_HARD_LIGHT] = combine_hard_light_float;
    imp->combine_float[PIXMAN_OP_SOFT_LIGHT] = combine_soft_light_float;
    imp->combine_float[PIXMAN_OP_DIFFERENCE] = combine_difference_float;
    imp->combine_float[PIXMAN_OP_EXCLUSION] = combine_exclusion_float;
    imp->combine_float[PIXMAN_OP_HSL_HUE] = combine_hsl_hue_float;
    imp->combine_float[PIXMAN_OP_HSL_SATURATION] = combine_hsl_saturation_float;
    imp->combine_float[PIXMAN_OP_HSL_COLOR] = combine_hsl_color_float;
    imp->combine_float[PIXMAN_OP_HSL_LUMINOSITY] = combine_hsl_luminosity_float;

    imp->combine_float_ca[PIXMAN_OP_CLEAR] = combine_clear_ca_float;
    imp->combine_float_ca[PIXMAN_OP_SRC] = combine_src_ca_float;
    imp->combine_float_ca[PIXMAN_OP_DST] = combine_dst_ca_float;
    imp->combine_float_ca[PIXMAN_OP_OVER] = combine_over_ca_float;
    imp->combine_float_ca[PIXMAN_OP_OVER_REVERSE] = combine_over_reverse_ca_float;
    imp->combine_float_ca[PIXMAN_OP_IN] = combine_in_ca_float;
    imp->combine_float_ca[PIXMAN_OP_IN_REVERSE] = combine_in_reverse_ca_float;
    imp->combine_float_ca[PIXMAN_OP_OUT] = combine_out_ca_float;
    imp->combine_float_ca[PIXMAN_OP_OUT_REVERSE] = combine_out_reverse_ca_float;
    imp->combine_float_ca[PIXMAN_OP_ATOP] = combine_atop_ca_float;
    imp->combine_float_ca[PIXMAN_OP_ATOP_REVERSE] = combine_atop_reverse_ca_float;
    imp->combine_float_ca[PIXMAN_OP_XOR] = combine_xor_ca_float;
    imp->combine_float_ca[PIXMAN_OP_ADD] = combine_add_ca_float;
    imp->combine_float_ca[PIXMAN_OP_SATURATE] = combine_saturate_ca_float;

    imp->combine_float_ca[PIXMAN_OP_DISJOINT_OVER] = combine_disjoint_over_ca_float;
    imp->combine_float_ca[PIXMAN_OP_DISJOINT_OVER_REVERSE] = combine_disjoint_over_reverse_ca_float;
    imp->combine_float_ca[PIXMAN_OP_DISJOINT_IN] = combine_disjoint_in_ca_float;
    imp->combine_float_ca[PIXMAN_OP_DISJOINT_IN_REVERSE] = combine_disjoint_in_reverse_ca_float;
    imp->combine_float_ca[PIXMAN_OP_DISJOINT_OUT] = combine_disjoint_out_ca_float;
    imp->combine_float_ca[PIXMAN_OP_DISJOINT_OUT_REVERSE] = combine_disjoint_out_reverse_ca_float;
    imp->combine_float_ca[PIXMAN_OP_DISJOINT_ATOP] = combine_disjoint_atop_ca_float;
    imp->combine_float_ca[PIXMAN_OP_DISJOINT_ATOP_REVERSE] = combine_disjoint_atop_reverse_ca_float;
    imp->combine_float_ca[PIXMAN_OP_DISJOINT_XOR] = combine_disjoint_xor_ca_float;

    imp->combine_float_ca[PIXMAN_OP_MULTIPLY] = combine_multiply_ca_float;
    imp->combine_float_ca[PIXMAN_OP_SCREEN] = combine_screen_ca_float;
    imp->combine_float_ca[PIXMAN_OP_OVERLAY] = combine_overlay_ca_float;
    imp->combine_float_ca[PIXMAN_OP_DARKEN] = combine_darken_ca_float;
    imp->combine_float_ca[PIXMAN_OP_LIGHTEN] = combine_lighten_ca_float;
    imp->combine_float_ca[PIXMAN_OP_COLOR_DODGE] = combine_color_dodge_ca_float;
    imp->combine_float_ca[PIXMAN_OP_COLOR_BURN] = combine_color_burn_ca_float;
    imp->combine_float_ca[PIXMAN_OP_HARD_LIGHT] = combine_hard_light_ca_float;
    imp->combine_float_ca[PIXMAN_OP_SOFT_LIGHT] = combine_soft_light_ca_float;
    imp->combine_float_ca[PIXMAN_OP_DIFFERENCE] = combine_difference_ca_float;
    imp->combine_float_ca[PIXMAN_OP_EXCLUSION] = combine_exclusion_ca_float;
    imp->combine_float_ca[PIXMAN_OP_HSL_HUE] = combine_hsl_hue_ca_float;
    imp->combine_float_ca[PIXMAN_OP_HSL_SATURATION] = combine_hsl_saturation_ca_float;
    imp->combine_float_ca[PIXMAN_OP_HSL_COLOR] = combine_hsl_color_ca_float;
    imp->combine_float_ca[PIXMAN_OP_HSL_LUMINOSITY] = combine_hsl_luminosity_ca_float;
}

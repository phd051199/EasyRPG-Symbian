/*
 * Copyright © 2000 SuSE, Inc.
 * Copyright © 2000 Keith Packard, member of The XFree86 Project, Inc.
 * Copyright © 2007 Red Hat, Inc.
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name of SuSE not be used in advertising or
 * publicity pertaining to distribution of the software without specific,
 * written prior permission.  SuSE makes no representations about the
 * suitability of this software for any purpose.  It is provided "as is"
 * without express or implied warranty.
 *
 * SuSE DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING ALL
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL SuSE
 * BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
 * OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <string.h>
#include <math.h>
#include <limits.h>
#include "pixman-private.h"
#include "pixman-combine32.h"

static void
combine_clear (pixman_implementation_t *imp,
               pixman_op_t              op,
               uint32_t *               dest,
               const uint32_t *         src,
               const uint32_t *         mask,
               int                      width)
{
    memset (dest, 0, width * sizeof(uint32_t));
}

static void
combine_dst (pixman_implementation_t *imp,
             pixman_op_t              op,
             uint32_t *               dest,
             const uint32_t *         src,
             const uint32_t *         mask,
             int                      width)
{
    return;
}

static void
combine_src_u (pixman_implementation_t *imp,
               pixman_op_t              op,
               uint32_t *               dest,
               const uint32_t *         src,
               const uint32_t *         mask,
               int                      width)
{
    memcpy (dest, src, width * sizeof(uint32_t));
}

static void
combine_over_u (pixman_implementation_t *imp,
                pixman_op_t              op,
                uint32_t *               dest,
                const uint32_t *         src,
                const uint32_t *         mask,
                int                      width)
{
    int i;

    for (i = 0; i < width; ++i)
    {
        uint32_t s = src[i];
        uint32_t a = ALPHA_8 (s);
        if (a == 0xff)
        {
            dest[i] = s;
        }
        else if (s)
        {
            uint32_t d = dest[i];
            a = a ^ 0xff;
            UN8x4_MUL_UN8_ADD_UN8x4 (d, a, s);
            dest[i] = d;
        }
    }
}

static void
combine_over_reverse_u (pixman_implementation_t *imp,
                        pixman_op_t              op,
                        uint32_t *               dest,
                        const uint32_t *         src,
                        const uint32_t *         mask,
                        int                      width)
{
    int i;

    for (i = 0; i < width; ++i)
    {
        uint32_t d = dest[i];
        uint32_t a = ALPHA_8 (d);
        if (a != 0xff)
        {
            uint32_t s = src[i];
            a = a ^ 0xff;
            UN8x4_MUL_UN8_ADD_UN8x4 (s, a, d);
            dest[i] = s;
        }
    }
}

static void
combine_in_u (pixman_implementation_t *imp,
              pixman_op_t              op,
              uint32_t *               dest,
              const uint32_t *         src,
              const uint32_t *         mask,
              int                      width)
{
    int i;

    for (i = 0; i < width; ++i)
    {
        uint32_t s = src[i];
        uint32_t a = ALPHA_8 (dest[i]);
        UN8x4_MUL_UN8 (s, a);
        dest[i] = s;
    }
}

static void
combine_in_reverse_u (pixman_implementation_t *imp,
                      pixman_op_t              op,
                      uint32_t *               dest,
                      const uint32_t *         src,
                      const uint32_t *         mask,
                      int                      width)
{
    int i;

    for (i = 0; i < width; ++i)
    {
        uint32_t d = dest[i];
        uint32_t a = ALPHA_8 (src[i]);
        UN8x4_MUL_UN8 (d, a);
        dest[i] = d;
    }
}

static void
combine_out_u (pixman_implementation_t *imp,
               pixman_op_t              op,
               uint32_t *               dest,
               const uint32_t *         src,
               const uint32_t *         mask,
               int                      width)
{
    int i;

    for (i = 0; i < width; ++i)
    {
        uint32_t s = src[i];
        uint32_t a = ALPHA_8 (dest[i]) ^ 0xff;
        UN8x4_MUL_UN8 (s, a);
        dest[i] = s;
    }
}

static void
combine_out_reverse_u (pixman_implementation_t *imp,
                       pixman_op_t              op,
                       uint32_t *               dest,
                       const uint32_t *         src,
                       const uint32_t *         mask,
                       int                      width)
{
    int i;

    for (i = 0; i < width; ++i)
    {
        uint32_t d = dest[i];
        uint32_t a = ALPHA_8 (src[i]) ^ 0xff;
        UN8x4_MUL_UN8 (d, a);
        dest[i] = d;
    }
}

static void
combine_atop_u (pixman_implementation_t *imp,
                pixman_op_t              op,
                uint32_t *               dest,
                const uint32_t *         src,
                const uint32_t *         mask,
                int                      width)
{
    int i;

    for (i = 0; i < width; ++i)
    {
        uint32_t s = src[i];
        uint32_t d = dest[i];
        uint32_t dest_a = ALPHA_8 (d);
        uint32_t src_ia = ALPHA_8 (s) ^ 0xff;
        UN8x4_MUL_UN8_ADD_UN8x4_MUL_UN8 (s, dest_a, d, src_ia);
        dest[i] = s;
    }
}

static void
combine_atop_reverse_u (pixman_implementation_t *imp,
                        pixman_op_t              op,
                        uint32_t *               dest,
                        const uint32_t *         src,
                        const uint32_t *         mask,
                        int                      width)
{
    int i;

    for (i = 0; i < width; ++i)
    {
        uint32_t s = src[i];
        uint32_t d = dest[i];
        uint32_t src_a = ALPHA_8 (s);
        uint32_t dest_ia = ALPHA_8 (d) ^ 0xff;
        UN8x4_MUL_UN8_ADD_UN8x4_MUL_UN8 (d, src_a, s, dest_ia);
        dest[i] = d;
    }
}

static void
combine_xor_u (pixman_implementation_t *imp,
               pixman_op_t              op,
               uint32_t *               dest,
               const uint32_t *         src,
               const uint32_t *         mask,
               int                      width)
{
    int i;

    for (i = 0; i < width; ++i)
    {
        uint32_t s = src[i];
        uint32_t d = dest[i];
        uint32_t src_ia = ALPHA_8 (s) ^ 0xff;
        uint32_t dest_ia = ALPHA_8 (d) ^ 0xff;
        UN8x4_MUL_UN8_ADD_UN8x4_MUL_UN8 (s, dest_ia, d, src_ia);
        dest[i] = s;
    }
}

static void
combine_add_u (pixman_implementation_t *imp,
               pixman_op_t              op,
               uint32_t *               dest,
               const uint32_t *         src,
               const uint32_t *         mask,
               int                      width)
{
    int i;

    for (i = 0; i < width; ++i)
    {
        uint32_t s = src[i];
        uint32_t d = dest[i];
        UN8x4_ADD_UN8x4 (d, s);
        dest[i] = d;
    }
}

static void
combine_saturate_u (pixman_implementation_t *imp,
                    pixman_op_t              op,
                    uint32_t *               dest,
                    const uint32_t *         src,
                    const uint32_t *         mask,
                    int                      width)
{
    int i;

    for (i = 0; i < width; ++i)
    {
        uint32_t s = src[i];
        uint32_t d = dest[i];
        uint32_t sa, da;

        sa = ALPHA_8 (s);
        if (sa == 0)
            continue;

        da = ALPHA_8 (d);
        if (sa + da >= 0xff)
        {
            sa = sa ? ((0xff - da) * 0xff / sa) : 0;
            UN8x4_MUL_UN8 (s, sa);
        }

        UN8x4_ADD_UN8x4 (d, s);
        dest[i] = d;
    }
}

#define MAKE_COMBINE_PD_U(name)                                         \
static void                                                             \
combine_ ## name ## _u (pixman_implementation_t *imp,                   \
                        pixman_op_t              op,                    \
                        uint32_t *               dest,                  \
                        const uint32_t *         src,                   \
                        const uint32_t *         mask,                  \
                        int                      width)                  \
{                                                                       \
    int i;                                                              \
                                                                        \
    for (i = 0; i < width; ++i)                                        \
    {                                                                   \
        uint32_t s = src[i];                                            \
        uint32_t d = dest[i];                                           \
        uint32_t src_a = ALPHA_8 (s);                                   \
        uint32_t dst_a = ALPHA_8 (d);                                   \
        uint32_t a = combine_ ## name ## _u_alpha (src_a, dst_a);       \
        UN8x4_MUL_UN8x4_ADD_UN8x4 (s, a, d);                            \
        dest[i] = s;                                                    \
    }                                                                   \
}

static force_inline uint32_t
combine_disjoint_over_u_alpha (uint32_t sa, uint32_t da)
{
    return da == 0 ? 0xff : DIV_UN8 (0xff - sa, da);
}

static force_inline uint32_t
combine_disjoint_over_reverse_u_alpha (uint32_t sa, uint32_t da)
{
    return sa == 0 ? 0xff : DIV_UN8 (0xff - da, sa);
}

static force_inline uint32_t
combine_disjoint_in_u_alpha (uint32_t sa, uint32_t da)
{
    return sa == 0 ? 0 : DIV_UN8 (da, sa);
}

static force_inline uint32_t
combine_disjoint_in_reverse_u_alpha (uint32_t sa, uint32_t da)
{
    return da == 0 ? 0 : DIV_UN8 (sa, da);
}

static force_inline uint32_t
combine_disjoint_out_u_alpha (uint32_t sa, uint32_t da)
{
    return sa == 0 ? 0xff : DIV_UN8 (0xff - da, sa);
}

static force_inline uint32_t
combine_disjoint_out_reverse_u_alpha (uint32_t sa, uint32_t da)
{
    return da == 0 ? 0xff : DIV_UN8 (0xff - sa, da);
}

static force_inline uint32_t
combine_disjoint_atop_u_alpha (uint32_t sa, uint32_t da)
{
    return sa == 0 ? 0 : DIV_UN8 (da, sa);
}

static force_inline uint32_t
combine_disjoint_atop_reverse_u_alpha (uint32_t sa, uint32_t da)
{
    return da == 0 ? 0 : DIV_UN8 (sa, da);
}

static force_inline uint32_t
combine_disjoint_xor_u_alpha (uint32_t sa, uint32_t da)
{
    return sa == 0 ? 0xff : DIV_UN8 (0xff - da, sa);
}

MAKE_COMBINE_PD_U (disjoint_over)
MAKE_COMBINE_PD_U (disjoint_over_reverse)
MAKE_COMBINE_PD_U (disjoint_in)
MAKE_COMBINE_PD_U (disjoint_in_reverse)
MAKE_COMBINE_PD_U (disjoint_out)
MAKE_COMBINE_PD_U (disjoint_out_reverse)
MAKE_COMBINE_PD_U (disjoint_atop)
MAKE_COMBINE_PD_U (disjoint_atop_reverse)
MAKE_COMBINE_PD_U (disjoint_xor)

static force_inline uint32_t
combine_conjoint_over_u_alpha (uint32_t sa, uint32_t da)
{
    return da == 0 ? 0xff : DIV_UN8 (0xff - sa, da);
}

static force_inline uint32_t
combine_conjoint_over_reverse_u_alpha (uint32_t sa, uint32_t da)
{
    return sa == 0 ? 0xff : DIV_UN8 (0xff - da, sa);
}

static force_inline uint32_t
combine_conjoint_in_u_alpha (uint32_t sa, uint32_t da)
{
    return sa == 0 ? 0 : DIV_UN8 (da, sa);
}

static force_inline uint32_t
combine_conjoint_in_reverse_u_alpha (uint32_t sa, uint32_t da)
{
    return da == 0 ? 0 : DIV_UN8 (sa, da);
}

static force_inline uint32_t
combine_conjoint_out_u_alpha (uint32_t sa, uint32_t da)
{
    return sa == 0 ? 0xff : DIV_UN8 (0xff - da, sa);
}

static force_inline uint32_t
combine_conjoint_out_reverse_u_alpha (uint32_t sa, uint32_t da)
{
    return da == 0 ? 0xff : DIV_UN8 (0xff - sa, da);
}

static force_inline uint32_t
combine_conjoint_atop_u_alpha (uint32_t sa, uint32_t da)
{
    return sa == 0 ? 0 : DIV_UN8 (da, sa);
}

static force_inline uint32_t
combine_conjoint_atop_reverse_u_alpha (uint32_t sa, uint32_t da)
{
    return da == 0 ? 0 : DIV_UN8 (sa, da);
}

static force_inline uint32_t
combine_conjoint_xor_u_alpha (uint32_t sa, uint32_t da)
{
    return sa == 0 ? 0xff : DIV_UN8 (0xff - da, sa);
}

MAKE_COMBINE_PD_U (conjoint_over)
MAKE_COMBINE_PD_U (conjoint_over_reverse)
MAKE_COMBINE_PD_U (conjoint_in)
MAKE_COMBINE_PD_U (conjoint_in_reverse)
MAKE_COMBINE_PD_U (conjoint_out)
MAKE_COMBINE_PD_U (conjoint_out_reverse)
MAKE_COMBINE_PD_U (conjoint_atop)
MAKE_COMBINE_PD_U (conjoint_atop_reverse)
MAKE_COMBINE_PD_U (conjoint_xor)

static void
combine_src_ca (pixman_implementation_t *imp,
                pixman_op_t              op,
                uint32_t *               dest,
                const uint32_t *         src,
                const uint32_t *         mask,
                int                      width)
{
    int i;

    for (i = 0; i < width; ++i)
    {
        uint32_t s = src[i];
        uint32_t m = mask[i];
        UN8x4_MUL_UN8x4 (s, m);
        dest[i] = s;
    }
}

static void
combine_over_ca (pixman_implementation_t *imp,
                 pixman_op_t              op,
                 uint32_t *               dest,
                 const uint32_t *         src,
                 const uint32_t *         mask,
                 int                      width)
{
    int i;

    for (i = 0; i < width; ++i)
    {
        uint32_t s = src[i];
        uint32_t m = mask[i];
        uint32_t a;
        uint32_t d = dest[i];

        UN8x4_MUL_UN8x4 (s, m);
        a = ALPHA_8 (s) ^ 0xff;
        UN8x4_MUL_UN8_ADD_UN8x4 (d, a, s);
        dest[i] = d;
    }
}

static void
combine_over_reverse_ca (pixman_implementation_t *imp,
                         pixman_op_t              op,
                         uint32_t *               dest,
                         const uint32_t *         src,
                         const uint32_t *         mask,
                         int                      width)
{
    int i;

    for (i = 0; i < width; ++i)
    {
        uint32_t s = src[i];
        uint32_t m = mask[i];
        uint32_t a = ALPHA_8 (dest[i]) ^ 0xff;
        uint32_t d = dest[i];

        UN8x4_MUL_UN8x4 (s, m);
        UN8x4_MUL_UN8_ADD_UN8x4 (s, a, d);
        dest[i] = s;
    }
}

static void
combine_in_ca (pixman_implementation_t *imp,
               pixman_op_t              op,
               uint32_t *               dest,
               const uint32_t *         src,
               const uint32_t *         mask,
               int                      width)
{
    int i;

    for (i = 0; i < width; ++i)
    {
        uint32_t s = src[i];
        uint32_t m = mask[i];
        uint32_t a = ALPHA_8 (dest[i]);

        UN8x4_MUL_UN8x4 (s, m);
        UN8x4_MUL_UN8 (s, a);
        dest[i] = s;
    }
}

static void
combine_in_reverse_ca (pixman_implementation_t *imp,
                       pixman_op_t              op,
                       uint32_t *               dest,
                       const uint32_t *         src,
                       const uint32_t *         mask,
                       int                      width)
{
    int i;

    for (i = 0; i < width; ++i)
    {
        uint32_t s = src[i];
        uint32_t m = mask[i];
        uint32_t d = dest[i];

        UN8x4_MUL_UN8x4 (s, m);
        UN8x4_MUL_UN8 (d, ALPHA_8 (s));
        dest[i] = d;
    }
}

static void
combine_out_ca (pixman_implementation_t *imp,
                pixman_op_t              op,
                uint32_t *               dest,
                const uint32_t *         src,
                const uint32_t *         mask,
                int                      width)
{
    int i;

    for (i = 0; i < width; ++i)
    {
        uint32_t s = src[i];
        uint32_t m = mask[i];
        uint32_t a = ALPHA_8 (dest[i]) ^ 0xff;

        UN8x4_MUL_UN8x4 (s, m);
        UN8x4_MUL_UN8 (s, a);
        dest[i] = s;
    }
}

static void
combine_out_reverse_ca (pixman_implementation_t *imp,
                        pixman_op_t              op,
                        uint32_t *               dest,
                        const uint32_t *         src,
                        const uint32_t *         mask,
                        int                      width)
{
    int i;

    for (i = 0; i < width; ++i)
    {
        uint32_t s = src[i];
        uint32_t m = mask[i];
        uint32_t d = dest[i];

        UN8x4_MUL_UN8x4 (s, m);
        UN8x4_MUL_UN8 (d, ALPHA_8 (s) ^ 0xff);
        dest[i] = d;
    }
}

static void
combine_atop_ca (pixman_implementation_t *imp,
                 pixman_op_t              op,
                 uint32_t *               dest,
                 const uint32_t *         src,
                 const uint32_t *         mask,
                 int                      width)
{
    int i;

    for (i = 0; i < width; ++i)
    {
        uint32_t s = src[i];
        uint32_t m = mask[i];
        uint32_t d = dest[i];
        uint32_t da = ALPHA_8 (d);

        UN8x4_MUL_UN8x4 (s, m);
        UN8x4_MUL_UN8_ADD_UN8x4_MUL_UN8 (s, da, d, ALPHA_8 (s) ^ 0xff);
        dest[i] = s;
    }
}

static void
combine_atop_reverse_ca (pixman_implementation_t *imp,
                         pixman_op_t              op,
                         uint32_t *               dest,
                         const uint32_t *         src,
                         const uint32_t *         mask,
                         int                      width)
{
    int i;

    for (i = 0; i < width; ++i)
    {
        uint32_t s = src[i];
        uint32_t m = mask[i];
        uint32_t d = dest[i];
        uint32_t da = ALPHA_8 (d) ^ 0xff;

        UN8x4_MUL_UN8x4 (s, m);
        UN8x4_MUL_UN8_ADD_UN8x4_MUL_UN8 (d, ALPHA_8 (s), s, da);
        dest[i] = d;
    }
}

static void
combine_xor_ca (pixman_implementation_t *imp,
                pixman_op_t              op,
                uint32_t *               dest,
                const uint32_t *         src,
                const uint32_t *         mask,
                int                      width)
{
    int i;

    for (i = 0; i < width; ++i)
    {
        uint32_t s = src[i];
        uint32_t m = mask[i];
        uint32_t d = dest[i];
        uint32_t da = ALPHA_8 (d) ^ 0xff;

        UN8x4_MUL_UN8x4 (s, m);
        UN8x4_MUL_UN8_ADD_UN8x4_MUL_UN8 (s, da, d, ALPHA_8 (s) ^ 0xff);
        dest[i] = s;
    }
}

static void
combine_add_ca (pixman_implementation_t *imp,
                pixman_op_t              op,
                uint32_t *               dest,
                const uint32_t *         src,
                const uint32_t *         mask,
                int                      width)
{
    int i;

    for (i = 0; i < width; ++i)
    {
        uint32_t s = src[i];
        uint32_t m = mask[i];
        uint32_t d = dest[i];

        UN8x4_MUL_UN8x4 (s, m);
        UN8x4_ADD_UN8x4 (d, s);
        dest[i] = d;
    }
}

static void
combine_saturate_ca (pixman_implementation_t *imp,
                     pixman_op_t              op,
                     uint32_t *               dest,
                     const uint32_t *         src,
                     const uint32_t *         mask,
                     int                      width)
{
    int i;

    for (i = 0; i < width; ++i)
    {
        uint32_t s = src[i];
        uint32_t m = mask[i];
        uint32_t d = dest[i];
        uint32_t sa, da;

        UN8x4_MUL_UN8x4 (s, m);
        sa = ALPHA_8 (s);
        if (sa == 0)
            continue;

        da = ALPHA_8 (d);
        if (sa + da >= 0xff)
        {
            sa = sa ? ((0xff - da) * 0xff / sa) : 0;
            UN8x4_MUL_UN8 (s, sa);
        }

        UN8x4_ADD_UN8x4 (d, s);
        dest[i] = d;
    }
}

static void
combine_multiply_ca (pixman_implementation_t *imp,
                     pixman_op_t              op,
                     uint32_t *               dest,
                     const uint32_t *         src,
                     const uint32_t *         mask,
                     int                      width)
{
    int i;

    for (i = 0; i < width; ++i)
    {
        uint32_t s = src[i];
        uint32_t m = mask[i];
        uint32_t d = dest[i];
        uint32_t a;

        UN8x4_MUL_UN8x4 (s, m);
        a = ALPHA_8 (s) ^ 0xff;
        UN8x4_MUL_UN8_ADD_UN8x4 (d, a, s);
        dest[i] = d;
    }
}

void
_pixman_setup_combiner_functions_32 (pixman_implementation_t *imp)
{
    imp->combine_32[PIXMAN_OP_CLEAR] = combine_clear;
    imp->combine_32[PIXMAN_OP_SRC] = combine_src_u;
    imp->combine_32[PIXMAN_OP_DST] = combine_dst;
    imp->combine_32[PIXMAN_OP_OVER] = combine_over_u;
    imp->combine_32[PIXMAN_OP_OVER_REVERSE] = combine_over_reverse_u;
    imp->combine_32[PIXMAN_OP_IN] = combine_in_u;
    imp->combine_32[PIXMAN_OP_IN_REVERSE] = combine_in_reverse_u;
    imp->combine_32[PIXMAN_OP_OUT] = combine_out_u;
    imp->combine_32[PIXMAN_OP_OUT_REVERSE] = combine_out_reverse_u;
    imp->combine_32[PIXMAN_OP_ATOP] = combine_atop_u;
    imp->combine_32[PIXMAN_OP_ATOP_REVERSE] = combine_atop_reverse_u;
    imp->combine_32[PIXMAN_OP_XOR] = combine_xor_u;
    imp->combine_32[PIXMAN_OP_ADD] = combine_add_u;
    imp->combine_32[PIXMAN_OP_SATURATE] = combine_saturate_u;

    imp->combine_32[PIXMAN_OP_DISJOINT_OVER] = combine_disjoint_over_u;
    imp->combine_32[PIXMAN_OP_DISJOINT_OVER_REVERSE] = combine_disjoint_over_reverse_u;
    imp->combine_32[PIXMAN_OP_DISJOINT_IN] = combine_disjoint_in_u;
    imp->combine_32[PIXMAN_OP_DISJOINT_IN_REVERSE] = combine_disjoint_in_reverse_u;
    imp->combine_32[PIXMAN_OP_DISJOINT_OUT] = combine_disjoint_out_u;
    imp->combine_32[PIXMAN_OP_DISJOINT_OUT_REVERSE] = combine_disjoint_out_reverse_u;
    imp->combine_32[PIXMAN_OP_DISJOINT_ATOP] = combine_disjoint_atop_u;
    imp->combine_32[PIXMAN_OP_DISJOINT_ATOP_REVERSE] = combine_disjoint_atop_reverse_u;
    imp->combine_32[PIXMAN_OP_DISJOINT_XOR] = combine_disjoint_xor_u;

    imp->combine_32[PIXMAN_OP_CONJOINT_OVER] = combine_conjoint_over_u;
    imp->combine_32[PIXMAN_OP_CONJOINT_OVER_REVERSE] = combine_conjoint_over_reverse_u;
    imp->combine_32[PIXMAN_OP_CONJOINT_IN] = combine_conjoint_in_u;
    imp->combine_32[PIXMAN_OP_CONJOINT_IN_REVERSE] = combine_conjoint_in_reverse_u;
    imp->combine_32[PIXMAN_OP_CONJOINT_OUT] = combine_conjoint_out_u;
    imp->combine_32[PIXMAN_OP_CONJOINT_OUT_REVERSE] = combine_conjoint_out_reverse_u;
    imp->combine_32[PIXMAN_OP_CONJOINT_ATOP] = combine_conjoint_atop_u;
    imp->combine_32[PIXMAN_OP_CONJOINT_ATOP_REVERSE] = combine_conjoint_atop_reverse_u;
    imp->combine_32[PIXMAN_OP_CONJOINT_XOR] = combine_conjoint_xor_u;

    imp->combine_32_ca[PIXMAN_OP_CLEAR] = combine_clear;
    imp->combine_32_ca[PIXMAN_OP_SRC] = combine_src_ca;
    imp->combine_32_ca[PIXMAN_OP_DST] = combine_dst;
    imp->combine_32_ca[PIXMAN_OP_OVER] = combine_over_ca;
    imp->combine_32_ca[PIXMAN_OP_OVER_REVERSE] = combine_over_reverse_ca;
    imp->combine_32_ca[PIXMAN_OP_IN] = combine_in_ca;
    imp->combine_32_ca[PIXMAN_OP_IN_REVERSE] = combine_in_reverse_ca;
    imp->combine_32_ca[PIXMAN_OP_OUT] = combine_out_ca;
    imp->combine_32_ca[PIXMAN_OP_OUT_REVERSE] = combine_out_reverse_ca;
    imp->combine_32_ca[PIXMAN_OP_ATOP] = combine_atop_ca;
    imp->combine_32_ca[PIXMAN_OP_ATOP_REVERSE] = combine_atop_reverse_ca;
    imp->combine_32_ca[PIXMAN_OP_XOR] = combine_xor_ca;
    imp->combine_32_ca[PIXMAN_OP_ADD] = combine_add_ca;
    imp->combine_32_ca[PIXMAN_OP_SATURATE] = combine_saturate_ca;
    imp->combine_32_ca[PIXMAN_OP_MULTIPLY] = combine_multiply_ca;
}

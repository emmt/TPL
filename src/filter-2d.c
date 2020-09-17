/*
 * filter-2d.c -
 *
 * Implementaion of simple separable 2D filters.
 *
 *-----------------------------------------------------------------------------
 *
 * This file is part of TPL software released under the MIT "Expat" license.
 *
 * Copyright (c) 2020: Éric Thiébaut <https://github.com/emmt/TPL>
 */

#ifndef _TPL_FILTER_2D_C
#define _TPL_FILTER_2D_C 1

#include "tpl-image.h"
#include "tpl-inline.h"

#define _tpl_index       long

/* Assume column-major storage order. */
#define dst(i1,i2)       dst[(i1) + dst_len1*(i2)]
#define src(i1,i2)       src[(i1) + src_len1*(i2)]

#define _tpl_float          float
#define _tpl_suffix         f
#define _tpl_public(name)   tpl_##name##_f
#define _tpl_private(name)  name##_f
#include __FILE__

#define _tpl_float          double
#define _tpl_suffix         d
#define _tpl_public(name)   tpl_##name##_d
#define _tpl_private(name)  name##_d
#include __FILE__

#else /* _TPL_FILTER_2D_C defined */

/*
 * Reference implementation of the filter.
 *
 * @param m    Number of filter coefficients.
 * @param n    Length of destination.
 * @param dst  Destination.
 * @param ker  Filter coefficients.
 * @param src  Source.
 */
static void
_tpl_private(filter_ref)(_tpl_index m,
                         _tpl_index n,
                         _tpl_float*restrict dst,
                         _tpl_float const*restrict ker,
                         _tpl_float const*restrict src)
{
    for (_tpl_index i = 0; i < n; ++i) {
        _tpl_float s = 0;
        for (_tpl_index k = 0; i < m; ++k) {
            s += ker[k]*src[i+k];
        }
        dst[i] = s;
    }
}

void
_tpl_public(filter_2d_ref)(int dim,
                           _tpl_float*restrict dst,
                           _tpl_index dst_len1,
                           _tpl_index dst_len2,
                           _tpl_float const*restrict ker,
                           _tpl_index ker_len,
                           _tpl_float const*restrict src,
                           _tpl_index src_len1,
                           _tpl_index src_len2,
                           _tpl_index k1,
                           _tpl_index k2,
                           _tpl_float*restrict wrk,
                           _tpl_float*restrict tmp)
{
    if (dim == 1) {
        _tpl_index wrk_len = dst_len1 + ker_len - 1;
        _tpl_index src_i2_prev = -1;
        for (_tpl_index dst_i2 = 0; dst_i2 < dst_len2; ++dst_i2) {
            _tpl_index src_i2 = dst_i2 + k2;
            if (src_i2 < 0) {
                src_i2 = 0;
            } else if (src_i2 >= src_len2) {
                src_i2 = src_len2 - 1;
            }
            if (src_i2 == src_i2_prev) {
                // Just copy previous result.
                tpl_copy_contiguous(dst_len1, &dst(0, dst_i2),
                                    &dst(0, dst_i2 - 1));
            } else {
                tpl_load_contiguous_flat(wrk_len, wrk,
                                         src_len1, &src(0, src_i2), k1);
                _tpl_private(filter_ref)(ker_len, dst_len1, &dst(0, dst_i2), ker, wrk);
                src_i2_prev = src_i2;
            }
        }
    } else {
        _tpl_index wrk_len = dst_len2 + ker_len - 1;
        _tpl_index src_i1_prev = -1;
        for (_tpl_index dst_i1 = 0; dst_i1 < dst_len1; ++dst_i1) {
            _tpl_index src_i1 = dst_i1 + k1;
            if (src_i1 < 0) {
                src_i1 = 0;
            } else if (src_i1 >= src_len1) {
                src_i1 = src_len1 - 1;
            }
            if (src_i1 == src_i1_prev) {
                // Just copy previous result.
                tpl_copy_strided(dst_len2, dst_len1,
                                 &dst(dst_i1, 0),
                                 &dst(dst_i1 - 1, 0));
            } else {
                tpl_load_strided_flat(wrk_len, wrk,
                                      src_len2, &src(src_i1, 0),
                                      k2, src_len1);

                _tpl_private(filter_ref)(ker_len, dst_len2, tmp, ker, wrk);

                tpl_store_strided(dst_len2, &dst(dst_i1, 0), dst_len1, tmp);

                src_i1_prev = src_i1;
            }
        }
    }
}

#undef _tpl_float
#undef _tpl_suffix
#undef _tpl_public
#undef _tpl_private

#endif /* _TPL_FILTER_2D_C */

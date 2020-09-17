/*
 * filter.c -
 *
 * Implementation of non-vectorized simple (i.e., linear, unidimensional,
 * compact and stationary) filters in TPL library.
 *
 *-----------------------------------------------------------------------------
 *
 * This file is part of TPL software released under the MIT "Expat" license.
 *
 * Copyright (c) 2020: Éric Thiébaut <https://github.com/emmt/TPL>
 *
 */

#ifndef _TPL_FILTER_C
#define _TPL_FILTER_C 1

#include "tpl-filter.h"

#define _tpl_index       long

#define _tpl_float       float
#define _tpl_func(name)  tpl_##name##_f
#include __FILE__

#define _tpl_float       double
#define _tpl_func(name)  tpl_##name##_d
#include __FILE__

#else /* _TPL_FILTER_C */

void
_tpl_func(filter)(_tpl_index                m,
                  _tpl_index                n,
                  _tpl_float      *restrict dst,
                  _tpl_float const*restrict ker,
                  _tpl_float const*restrict src)
{
    if (m == 5) {
        _tpl_func(filter_x5)(n, dst, ker, src);
    } else if (m == 4) {
        _tpl_func(filter_x4)(n, dst, ker, src);
    } else if (m == 3) {
        _tpl_func(filter_x3)(n, dst, ker, src);
    } else if (m == 2) {
        _tpl_func(filter_x2)(n, dst, ker, src);
    } else if (m == 1) {
        _tpl_func(filter_x1)(n, dst, ker, src);
    } else {
        _tpl_func(filter_ref)(m, n, dst, ker, src);
    }
}

void
_tpl_func(filter_ref)(_tpl_index                m,
                      _tpl_index                n,
                      _tpl_float      *restrict dst,
                      _tpl_float const*restrict ker,
                      _tpl_float const*restrict src)
{
    for (_tpl_index i = 0; i < n; ++i) {
        _tpl_float s = 0;
        for (_tpl_index k = 0; k < m; ++k) {
            s += ker[k]*src[i+k];
        }
        dst[i] = s;
    }
}

#undef _tpl_float
#undef _tpl_func

#endif /* _TPL_FILTER_C */

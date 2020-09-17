/*
 * tpl-inline.h -
 *
 * Definitions for inline code in TPL library.
 *
 *-----------------------------------------------------------------------------
 *
 * This file is part of TPL software released under the MIT "Expat" license.
 *
 * Copyright (c) 2020: Éric Thiébaut <https://github.com/emmt/TPL>
 *
 */

#ifndef _TPL_INLINE_H
#define _TPL_INLINE_H 1

#include <pvc-meta.h>
#include <pvc-math.h>

/**
 * @def tpl_copy_contiguous(len,dst,src)
 *
 * @brief Copy consecutive contiguous values.
 *
 * The call `tpl_copy_contiguous(len,dst,src)` expands to inline code for
 * copying `len` consecutive values from the source `src` to the destination
 * `dst`.  Consecutive values are assumed contiguous in the source and in the
 * destination.
 *
 * The call `tpl_copy_contiguous(len,dst,src)` expands to inline code which is
 * equivalent to:
 *
 * ```.c
 * for (long i = 0; i < len; ++i) {
 *     dst[i] = src[i];
 * }
 * ```
 *
 * @param len   Number of elements to copy.
 * @param dst   Address of first element of destination array.
 * @param src   Address of first element of source array.
 *
 * @see tpl_copy_strided, tpl_load_strided, tpl_store_strided.
 */
#ifdef _TPL_DOXYGEN_PARSING

#define tpl_copy_contiguous(len, dst, src) ...

#else /* _TPL_DOXYGEN_PARSING not defined */

#define tpl_copy_contiguous(len, dst, src)                      \
    _Generic(*(dst),                                            \
             float:  tpl_copy_contiguous_f,                     \
             double: tpl_copy_contiguous_d)(len, dst, src)

#endif /* _TPL_DOXYGEN_PARSING */

/**
 * @def tpl_copy_strided(len, inc, dst, src)
 *
 * @brief Copy consecutive strided values.
 *
 * The call `tpl_copy_strided(len,inc,dst,src)` expands to inline code for
 * copying `len` consecutive values from the source `src` to the destination
 * `dst`.  Argument `inc` is the increment between consecutive elements in the
 * source and in the destination.  If `inc=1`, call
 * `tpl_copy_contiguous(len,dst,src)` instead.
 *
 * The call `tpl_copy_strided(len,inc,dst,src)` expands to inline code which is
 * equivalent to:
 *
 * ```.c
 * for (long i = 0; i < len; ++i) {
 *     long j = i*inc;
 *     dst[j] = src[j];
 * }
 * ```
 *
 * @param len   Number of elements to copy.
 * @param inc   Index increment in the destination and source arrayq.
 * @param dst   Address of first element of destination array.
 * @param src   Address of first element of source array.
 *
 * @see tpl_copy_contiguous, tpl_load_strided, tpl_store_strided.
 */
#ifdef _TPL_DOXYGEN_PARSING

#define tpl_copy_strided(len, inc, dst, src) ...

#else /* _TPL_DOXYGEN_PARSING not defined */

#define tpl_copy_strided(len, inc, dst, src)                    \
    _Generic(*(dst),                                            \
             float:  tpl_copy_strided_f,                        \
             double: tpl_copy_strided_d)(len, inc, dst, src)

#endif /* _TPL_DOXYGEN_PARSING */

/**
 * @def tpl_load_strided(len, dst, src, inc)
 *
 * @brief Load consecutive strided values.
 *
 * The call `tpl_load_strided(len,dst,src,inc)` expands to inline code for
 * copying `len` consecutive values from the source `src` to the destination
 * `dst`.  Argument `inc` is the increment between consecutive elements in the
 * source.  Values are assumed contiguous in the destination.  If `inc=1`, call
 * `tpl_copy_contiguous(len,dst,src)` instead.
 *
 * The call `tpl_load_strided(len,dst,src,inc)` expands to inline code which is
 * equivalent to:
 *
 * ```.c
 * for (long i = 0; i < len; ++i) {
 *     dst[i] = src[i*inc];
 * }
 * ```
 *
 * @param len   Number of elements to copy.
 * @param dst   Address of first element of destination array.
 * @param src   Address of first element of source array.
 * @param inc   Index increment in the source array.
 *
 * @see tpl_copy_contiguous, tpl_copy_strided, tpl_store_strided.
 */
#ifdef _TPL_DOXYGEN_PARSING

#define tpl_load_strided(len, dst, src, inc) ...

#else /* _TPL_DOXYGEN_PARSING not defined */

#define tpl_load_strided(len, dst, src, inc)                    \
    _Generic(*(dst),                                            \
             float:  tpl_load_strided_f,                        \
             double: tpl_load_strided_d)(len, dst, src, inc)

#endif /* _TPL_DOXYGEN_PARSING */

/**
 * @def tpl_store_strided(len, dst, inc, src)
 *
 * @brief Store consecutive strided values.
 *
 * The call `tpl_store_strided(len,dst,inc,src)` expands to inline code for
 * copying `len` consecutive values from the source `src` to the destination
 * `dst`.  Argument `inc` is the increment between consecutive elements in the
 * destination.  Values are assumed contiguous in the source.  If `inc=1`, call
 * `tpl_copy_contiguous(len,dst,src)` instead.
 *
 * The call `tpl_store_strided(len,dst,inc,src)` expands to inline code which
 * is equivalent to:
 *
 * ```.c
 * for (long i = 0; i < len; ++i) {
 *     dst[i*inc] = src[i];
 * }
 * ```
 *
 * @param len   Number of elements to copy.
 * @param dst   Address of first element of destination array.
 * @param inc   Index increment in the destination array.
 * @param src   Address of first element of source array.
 *
 * @see tpl_copy_contiguous, tpl_copy_strided, tpl_load_strided.
 */
#ifdef _TPL_DOXYGEN_PARSING

#define tpl_store_strided(len, dst, inc, src) ...

#else /* _TPL_DOXYGEN_PARSING not defined */

#define tpl_store_strided(len, dst, inc, src)                   \
    _Generic(*(dst),                                            \
             float:  tpl_store_strided_f,                       \
             double: tpl_store_strided_d)(len, dst, inc, src)

#endif /* _TPL_DOXYGEN_PARSING */

/**
 * @def tpl_load_contiguous_flat(m, y, n, x, k)
 *
 * @brief Load contiguous values assuming flat boundary conditions.
 *
 * Copy contiguous values from a source array into a destination array with
 * an offset and assuming *flat* boundary conditions.  Values are stored
 * contiguously in the destination array.
 *
 * The call `tpl_load_contiguous_flat(m,y,n,x,k)` expands to inline code which
 * is equivalent to:
 *
 * ```.c
 * for (long i = 0; i < m; ++i) {
 *     long j = clamp(i + off, 0, srclen - 1);
 *     dst[i] = src[j];
 * }
 * ```
 *
 * where `clamp(a,lo,hi)` yields `min(max(a,lo),hi)`.
 *
 * @param m   Number of elements to copy.
 * @param y   Address of first element of destination array.
 * @param n   Number of elements in source array.
 * @param x   Address of first element of source array.
 * @param k   Index offset.
 *
 * @see tpl_load_strided_flat.
 */
#ifdef _TPL_DOXYGEN_PARSING

#define tpl_load_contiguous_flat(m, y, n, x, k) ...

#else /* _TPL_DOXYGEN_PARSING not defined */

#define tpl_load_contiguous_flat(m, y, n, x, k)                 \
    _Generic(*(y),                                              \
             float:  tpl_load_contiguous_flat_f,                \
             double: tpl_load_contiguous_flat_d)(m, y, n, x, k)

#endif /* _TPL_DOXYGEN_PARSING */

/**
 * @def tpl_load_strided_flat(m, y, n, x, k, s)
 *
 * @brief Load strided values assuming flat boundary conditions.
 *
 * Copy strided values from a source array into a destination array with an
 * offset and assuming *flat* boundary conditions.  Values are stored
 * contiguously in the destination array and have a given stride in the source
 * array.
 *
 * For unit strided load (i.e., `s = 1`), call tpl_load_contiguous_flat()
 * instead.
 *
 * The call `tpl_load_strided_flat(m,y,n,x,k,s)` expands to inline code which
 * is equivalent to:
 *
 * ```.c
 * for (long i = 0; i < m; ++i) {
 *     long j = clamp(i + off, 0, srclen - 1);
 *     dst[i] = src[j*s];
 * }
 * ```
 *
 * where `clamp(a,lo,hi)` yields `min(max(a,lo),hi)`.
 *
 * @param m   Number of elements to copy.
 * @param y   Address of first element of destination array.
 * @param n   Number of strided elements in source array.
 * @param x   Address of first element of source array.
 * @param k   Index offset.
 * @param s   Index increment in the source array.
 *
 * @see tpl_load_contiguous_flat.
 */
#ifdef _TPL_DOXYGEN_PARSING

#define tpl_load_strided_flat(m, y, n, x, k, s) ...

#else /* _TPL_DOXYGEN_PARSING not defined */

#define tpl_load_strided_flat(m, y, n, x, k, s)                 \
    _Generic(*(y),                                              \
             float:  tpl_load_strided_flat_f,                   \
             double: tpl_load_strided_flat_d)(m, y, n, x, k, s)

#endif /* _TPL_DOXYGEN_PARSING */

#ifndef _TPL_DOXYGEN_PARSING

#define _TPL_DEFINE_INLINE_FUNCTIONS 1

#define _tpl_type          float
#define _tpl_func(func)    tpl_ ## func ## _f
#include __FILE__

#define _tpl_type          double
#define _tpl_func(func)    tpl_ ## func ## _d
#include __FILE__

#undef _TPL_DEFINE_INLINE_FUNCTIONS

#endif /* _TPL_DOXYGEN_PARSING */

#else  /* _TPL_INLINE_H */

/*
 * The remaining part of this file may be included multiple times with
 * different definitions of the macros `_tpl_type` and `_tpl_func` to generate
 * the code of inlined functions for different element types.
 *
 * The ugliness of the inlined code is to avoid that macro definitions can
 * break the code.  It is assumed that the polite user consider as *private*
 * names prefixed by `_tpl_` or `_TPL_`.
 */

#ifdef _TPL_DEFINE_INLINE_FUNCTIONS

static inline void
_tpl_func(copy_contiguous)(long _tpl_len,
                           _tpl_type*restrict _tpl_dst,
                           _tpl_type const*restrict _tpl_src)
{
    for (long _tpl_i = 0; _tpl_i < _tpl_len; ++_tpl_i) {
        _tpl_dst[_tpl_i] = _tpl_src[_tpl_i];
    }
}

static inline void
_tpl_func(copy_strided)(long _tpl_len,
                        long _tpl_inc,
                        _tpl_type*restrict _tpl_dst,
                        _tpl_type const*restrict _tpl_src)
{
    for (long _tpl_i = 0; _tpl_i < _tpl_len; ++_tpl_i) {
        long _tpl_j =_tpl_i*_tpl_inc;
        _tpl_dst[_tpl_j] = _tpl_src[_tpl_j];
    }
}

static inline void
_tpl_func(load_strided)(long _tpl_len,
                        _tpl_type*restrict _tpl_dst,
                        _tpl_type const*restrict _tpl_src,
                        long _tpl_inc)
{
    for (long _tpl_i = 0; _tpl_i < _tpl_len; ++_tpl_i) {
        _tpl_dst[_tpl_i] = _tpl_src[_tpl_i*_tpl_inc];
    }
}

static inline void
_tpl_func(store_strided)(long _tpl_len,
                         _tpl_type*restrict _tpl_dst,
                         long _tpl_inc,
                         _tpl_type const*restrict _tpl_src)
{
    for (long _tpl_i = 0; _tpl_i < _tpl_len; ++_tpl_i) {
        _tpl_dst[_tpl_i*_tpl_inc] = _tpl_src[_tpl_i];
    }
}

static inline void
_tpl_func(load_contiguous_flat)(long                     _tpl_m,
                                _tpl_type*restrict       _tpl_y,
                                long                     _tpl_n,
                                _tpl_type const*restrict _tpl_x,
                                long                     _tpl_k)
{
    long _tpl_i1 = pvc_max(-_tpl_k, 0);
    long _tpl_i2 = pvc_min(_tpl_n - _tpl_k, _tpl_m);
    if (_tpl_i1 > 0) {
        _tpl_type _tpl_v = _tpl_x[0];
        long _tpl_i0 = pvc_min(_tpl_i1, _tpl_m);
        for (long _tpl_i = 0; _tpl_i < _tpl_i0; ++_tpl_i) {
            _tpl_y[_tpl_i] = _tpl_v;
        }
    }
    for (long _tpl_i = _tpl_i1; _tpl_i < _tpl_i2; ++_tpl_i) {
        _tpl_y[_tpl_i] = _tpl_x[_tpl_i + _tpl_k];
    }
    if (_tpl_i2 < _tpl_m) {
        _tpl_type _tpl_v = _tpl_x[_tpl_n - 1];
        long _tpl_i3 = pvc_max(_tpl_i2, 0);
        for (long _tpl_i = _tpl_i3; _tpl_i < _tpl_m; ++_tpl_i) {
            _tpl_y[_tpl_i] = _tpl_v;
        }
    }
}

static inline void
_tpl_func(load_strided_flat)(long                     _tpl_m,
                             _tpl_type*restrict       _tpl_y,
                             long                     _tpl_n,
                             _tpl_type const*restrict _tpl_x,
                             long                     _tpl_k,
                             long                     _tpl_s)
{
    long _tpl_i1 = pvc_max(-_tpl_k, 0);
    long _tpl_i2 = pvc_min(_tpl_n - _tpl_k, _tpl_m);
    if (_tpl_i1 > 0) {
        _tpl_type _tpl_v = _tpl_x[0];
        long _tpl_i0 = pvc_min(_tpl_i1, _tpl_m);
        for (long _tpl_i = 0; _tpl_i < _tpl_i0; ++_tpl_i) {
            _tpl_y[_tpl_i] = _tpl_v;
        }
    }
    for (long _tpl_i = _tpl_i1; _tpl_i < _tpl_i2; ++_tpl_i) {
        _tpl_y[_tpl_i] = _tpl_x[(_tpl_i + _tpl_k)*_tpl_s];
    }
    if (_tpl_i2 < _tpl_m) {
        _tpl_type _tpl_v = _tpl_x[(_tpl_n - 1)*_tpl_s];
        long _tpl_i3 = pvc_max(_tpl_i2, 0);
        for (long _tpl_i = _tpl_i3; _tpl_i < _tpl_m; ++_tpl_i) {
            _tpl_y[_tpl_i] = _tpl_v;
        }
    }
}

#undef _tpl_type
#undef _tpl_func

#endif /* _TPL_DEFINE_INLINE_FUNCTIONS */

#endif /* _TPL_INLINE_H */

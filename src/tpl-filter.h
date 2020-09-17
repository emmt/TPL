/*
 * tpl-filter.h -
 *
 * Definitions for simple (i.e., linear, unidimensional, compact and
 * stationary) filter code in TPL library.
 *
 *-----------------------------------------------------------------------------
 *
 * This file is part of TPL software released under the MIT "Expat" license.
 *
 * Copyright (c) 2020: Éric Thiébaut <https://github.com/emmt/TPL>
 *
 */

#ifndef _TPL_FILTER_H
#define _TPL_FILTER_H 1

#ifdef __cplusplus
#  define _TPL_EXTERN_C_BEGIN extern "C" {
#  define _TPL_EXTERN_C_END   }
#  if defined(__GNUC__) || defined(__clang__)
#    define restrict __restrict__
#  elif defined(_MSC_VER)
#    define restrict __restrict
#  else
#    define restrict
#  endif
#else
#  define _TPL_EXTERN_C_BEGIN
#  define _TPL_EXTERN_C_END
#endif

_TPL_EXTERN_C_BEGIN

/**
 * @def tpl_filter(m,n,dst,ker,src)
 *
 * @brief Apply simple (i.e., linear, unidimensional, compact and
 * stationary) filter.
 *
 * The call `tpl_filter(m,n,dst,ker,src)` is equivalent to the following
 * pseudo-code:
 *
 * ```.c
 * for (long i = 0; i < n; ++i) {
 *     dst[i] = ker[0]*src[i] + ker[1]*src[i+1] + ... + ker[m-1]*src[i+m-1];
 *     T s = 0;
 *     for (long k = 0; k < m; ++k) {
 *         s += ker[k]*src[i+k];
 *     }
 *     dst[i] = s;
 * }
 *
 * @param m     Number of coefficients in kernel.
 * @param n     Number of elements in destination.
 * @param dst   Destination array.  Must have at least `n` elements.
 * @param ker   Kernel coefficients.  Must have at least `m` elements.
 * @param src   Source array. Must have at least `m + n - 1` elements.
 */
#define tpl_filter(m,n,dst,ker,src)                     \
    _Generic(*(dst),                                    \
             float:  tpl_filter_f,                      \
             double: tpl_filter_d)(m,n,dst,ker,src)

#define tpl_filter_ref(m,n,dst,ker,src)                 \
    _Generic(*(dst),                                    \
             float:  tpl_filter_ref_f,                  \
             double: tpl_filter_ref_d)(m,n,dst,ker,src)

#define tpl_filter_x1(n,dst,ker,src)                    \
    _Generic(*(dst),                                    \
             float:  tpl_filter_x1_f,                   \
             double: tpl_filter_x1_d)(n,dst,ker,src)

#define tpl_filter_x2(n,dst,ker,src)                    \
    _Generic(*(dst),                                    \
             float:  tpl_filter_x2_f,                   \
             double: tpl_filter_x2_d)(n,dst,ker,src)

#define tpl_filter_x3(n,dst,ker,src)                    \
    _Generic(*(dst),                                    \
             float:  tpl_filter_x3_f,                   \
             double: tpl_filter_x3_d)(n,dst,ker,src)

#define tpl_filter_x4(n,dst,ker,src)                    \
    _Generic(*(dst),                                    \
             float:  tpl_filter_x4_f,                   \
             double: tpl_filter_x4_d)(n,dst,ker,src)

#define tpl_filter_x5(n,dst,ker,src)                    \
    _Generic(*(dst),                                    \
             float:  tpl_filter_x5_f,                   \
             double: tpl_filter_x5_d)(n,dst,ker,src)

// Single precision versions.
extern void tpl_filter_f(long m,
                         long n,
                         float *restrict dst,
                         float const*restrict ker,
                         float const*restrict src);
extern void tpl_filter_ref_f(long m,
                             long n,
                             float *restrict dst,
                             float const*restrict ker,
                             float const*restrict src);
extern void tpl_filter_x1_f(long n,
                            float *restrict dst,
                            float const*restrict ker,
                            float const*restrict src);
extern void tpl_filter_x2_f(long n,
                            float *restrict dst,
                            float const*restrict ker,
                            float const*restrict src);
extern void tpl_filter_x3_f(long n,
                            float *restrict dst,
                            float const*restrict ker,
                            float const*restrict src);
extern void tpl_filter_x4_f(long n,
                            float *restrict dst,
                            float const*restrict ker,
                            float const*restrict src);
extern void tpl_filter_x5_f(long n,
                            float *restrict dst,
                            float const*restrict ker,
                            float const*restrict src);

// Double precision versions.
extern void tpl_filter_d(long m,
                         long n,
                         double *restrict dst,
                         double const*restrict ker,
                         double const*restrict src);
extern void tpl_filter_ref_d(long m,
                             long n,
                             double *restrict dst,
                             double const*restrict ker,
                             double const*restrict src);
extern void tpl_filter_x1_d(long n,
                            double *restrict dst,
                            double const*restrict ker,
                            double const*restrict src);
extern void tpl_filter_x2_d(long n,
                            double *restrict dst,
                            double const*restrict ker,
                            double const*restrict src);
extern void tpl_filter_x3_d(long n,
                            double *restrict dst,
                            double const*restrict ker,
                            double const*restrict src);
extern void tpl_filter_x4_d(long n,
                            double *restrict dst,
                            double const*restrict ker,
                            double const*restrict src);
extern void tpl_filter_x5_d(long n,
                            double *restrict dst,
                            double const*restrict ker,
                            double const*restrict src);
_TPL_EXTERN_C_END

#endif /* _TPL_FILTER_H */

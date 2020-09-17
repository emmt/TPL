/*
 * tpl-image.h -
 *
 * Definitions for image processing in TPL library.
 *
 *-----------------------------------------------------------------------------
 *
 * This file is part of TPL software released under the MIT "Expat" license.
 *
 * Copyright (c) 2020: Éric Thiébaut <https://github.com/emmt/TPL>
 */

#ifndef _TPL_IMAGE_H
#define _TPL_IMAGE_H 1

#include <pvc.h>

_PVC_EXTERN_C_BEGIN

/**
 * Apply a simple filter along a dimension of an image.
 *
 * Column-major storage order is assumed for multi-dimensional arrays.
 *
 * @param dim        Dimension of interest (1 or 2).
 * @param dst        Destination array.
 * @param dst_len1   Length of 1st dimension of destination array.
 * @param dst_len2   Length of 2nd dimension of destination array.
 * @param ker        Filter coefficients.
 * @param ker_len    Number of filter coefficients.
 * @param src        Source array.
 * @param src_len1   Length of 1st dimension of source array.
 * @param src_len2   Length of 2nd dimension of source array.
 * @param k1         Offset along 1st dimension.
 * @param k2         Offset along 2nd dimension.
 * @param wrk1       Primary workspace.  Must have at least
 *                   `dst_len + ker_len - 1` elements with `dst_len`
 *                   the length of the dimension of interest in the
 *                   destination array (i.e., `dst_len = dst_len1` if
 *                   `dim = 1` or `dst_len = dst_len1` if `dim = 2`).
 * @param wrk2       Secondary workspace.  Unused (can be `NULL`) if
 *                   `dim = 1`, must have at least `dst_len2` elements
 *                   if `dim = 2`.
 */
#define tpl_filter_2d(dim, dst, dst_len1, dst_len2,     \
                      ker, ker_len,                     \
                      src, src_len1, src_len2,          \
                      k1, k2, wrk1, wrk2)               \
    _Generic(*(dst),                                    \
             float:  tpl_filter_2d_f,                   \
             double: tpl_filter_2d_d)                   \
    (dim, dst, dst_len1, dst_len2, ker,                 \
     src, src_len1, src_len2, k1, k2, wrk1, wrk2)

extern void
tpl_filter_2d_f(int dim,
                float*restrict dst,
                long dst_len1,
                long dst_len2,
                float const*restrict ker,
                long ker_len,
                float const*restrict src,
                long src_len1,
                long src_len2,
                long k1,
                long k2,
                float*restrict wrk1,
                float*restrict wrk2);

extern void
tpl_filter_2d_d(int dim,
                double*restrict dst,
                long dst_len1,
                long dst_len2,
                double const*restrict ker,
                long ker_len,
                double const*restrict src,
                long src_len1,
                long src_len2,
                long k1,
                long k2,
                double*restrict wrk1,
                double*restrict wrk2);

/**
 * Apply a simple filter along a dimension of an image.
 *
 * This function is the same as tpl_filter_2d() but is not optimized and is
 * intended to serve as a reference implementation.
 *
 * @param dim        Dimension of interest (1 or 2).
 * @param dst        Destination array.
 * @param dst_len1   Length of 1st dimension of destination array.
 * @param dst_len2   Length of 2nd dimension of destination array.
 * @param ker        Filter coefficients.
 * @param ker_len    Number of filter coefficients.
 * @param src        Source array.
 * @param src_len1   Length of 1st dimension of source array.
 * @param src_len2   Length of 2nd dimension of source array.
 * @param k1         Offset along 1st dimension.
 * @param k2         Offset along 2nd dimension.
 * @param wrk1       Primary workspace.  Must have at least
 *                   `dst_len + ker_len - 1` elements with `dst_len`
 *                   the length of the dimension of interest in the
 *                   destination array (i.e., `dst_len = dst_len1` if
 *                   `dim = 1` or `dst_len = dst_len1` if `dim = 2`).
 * @param wrk2       Secondary workspace.  Unused (can be `NULL`) if
 *                   `dim = 1`, must have at least `dst_len2` elements
 *                   if `dim = 2`.
 */
#define tpl_filter_ref_2d(dim, dst, dst_len1, dst_len2, \
                          ker, ker_len,                 \
                          src, src_len1, src_len2,      \
                          k1, k2, wrk1, wrk2)           \
    _Generic(*(dst),                                    \
             float:  tpl_filter_2d_ref_f,               \
             double: tpl_filter_2d_ref_d)               \
    (dim, dst, dst_len1, dst_len2, ker,                 \
     src, src_len1, src_len2, k1, k2, wrk1, wrk2)

extern void
tpl_filter_2d_ref_f(int dim,
                    float*restrict dst,
                    long dst_len1,
                    long dst_len2,
                    float const*restrict ker,
                    long ker_len,
                    float const*restrict src,
                    long src_len1,
                    long src_len2,
                    long k1,
                    long k2,
                    float*restrict wrk1,
                    float*restrict wrk2);

extern void
tpl_filter_2d_ref_d(int dim,
                    double*restrict dst,
                    long dst_len1,
                    long dst_len2,
                    double const*restrict ker,
                    long ker_len,
                    double const*restrict src,
                    long src_len1,
                    long src_len2,
                    long k1,
                    long k2,
                    double*restrict wrk1,
                    double*restrict wrk2);

/*
 * Nomenclature for specialized 2D separable linear filters.
 *
 * Functions are named as `tpl_filter_2d_xSIZ_DIM_SFX` where `2d` means
 * *2-dimensional* (i.e., images) and with `SIZ` the size of the kernel, `DIM`
 * the dimension to consider (i.e., `1st` or `2nd`) and `SFX` the type suffix
 * (`f` for `float` and `d` for `double`).
 */

/* Single precision. */

extern void
tpl_filter_2d_1st_f(float*restrict dst,
                    long dst_len1,
                    long dst_len2,
                    float const*restrict ker,
                    long ker_len,
                    float const*restrict src,
                    long src_len1,
                    long src_len2,
                    long k1,
                    long k2,
                    float*restrict wrk);

extern void
tpl_filter_2d_2nd_f(float*restrict dst,
                    long dst_len1,
                    long dst_len2,
                    float const*restrict ker,
                    long ker_len,
                    float const*restrict src,
                    long src_len1,
                    long src_len2,
                    long k1,
                    long k2,
                    float*restrict wrk1,
                    float*restrict wrk2);

extern void
tpl_filter_2d_x1_1st_f(float*restrict dst,
                      long dst_len1,
                      long dst_len2,
                      float const*restrict ker,
                      float const*restrict src,
                      long src_len1,
                      long src_len2,
                      long k1,
                      long k2,
                      float*restrict wrk);

extern void
tpl_filter_2d_x1_2nd_f(float*restrict dst,
                      long dst_len1,
                      long dst_len2,
                      float const*restrict ker,
                      float const*restrict src,
                      long src_len1,
                      long src_len2,
                      long k1,
                      long k2,
                      float*restrict wrk1,
                      float*restrict wrk2);

extern void
tpl_filter_2d_x2_1st_f(float*restrict dst,
                      long dst_len1,
                      long dst_len2,
                      float const*restrict ker,
                      float const*restrict src,
                      long src_len1,
                      long src_len2,
                      long k1,
                      long k2,
                      float*restrict wrk);

extern void
tpl_filter_2d_x2_2nd_f(float*restrict dst,
                      long dst_len1,
                      long dst_len2,
                      float const*restrict ker,
                      float const*restrict src,
                      long src_len1,
                      long src_len2,
                      long k1,
                      long k2,
                      float*restrict wrk1,
                      float*restrict wrk2);

extern void
tpl_filter_2d_x3_1st_f(float*restrict dst,
                      long dst_len1,
                      long dst_len2,
                      float const*restrict ker,
                      float const*restrict src,
                      long src_len1,
                      long src_len2,
                      long k1,
                      long k2,
                      float*restrict wrk);

extern void
tpl_filter_2d_x3_2nd_f(float*restrict dst,
                      long dst_len1,
                      long dst_len2,
                      float const*restrict ker,
                      float const*restrict src,
                      long src_len1,
                      long src_len2,
                      long k1,
                      long k2,
                      float*restrict wrk1,
                      float*restrict wrk2);

extern void
tpl_filter_2d_x4_1st_f(float*restrict dst,
                      long dst_len1,
                      long dst_len2,
                      float const*restrict ker,
                      float const*restrict src,
                      long src_len1,
                      long src_len2,
                      long k1,
                      long k2,
                      float*restrict wrk);

extern void
tpl_filter_2d_x4_2nd_f(float*restrict dst,
                      long dst_len1,
                      long dst_len2,
                      float const*restrict ker,
                      float const*restrict src,
                      long src_len1,
                      long src_len2,
                      long k1,
                      long k2,
                      float*restrict wrk1,
                      float*restrict wrk2);

extern void
tpl_filter_2d_x5_1st_f(float*restrict dst,
                      long dst_len1,
                      long dst_len2,
                      float const*restrict ker,
                      float const*restrict src,
                      long src_len1,
                      long src_len2,
                      long k1,
                      long k2,
                      float*restrict wrk);

extern void
tpl_filter_2d_x5_2nd_f(float*restrict dst,
                      long dst_len1,
                      long dst_len2,
                      float const*restrict ker,
                      float const*restrict src,
                      long src_len1,
                      long src_len2,
                      long k1,
                      long k2,
                      float*restrict wrk1,
                      float*restrict wrk2);

/* Double precision. */

extern void
tpl_filter_2d_1st_d(double*restrict dst,
                    long dst_len1,
                    long dst_len2,
                    double const*restrict ker,
                    long ker_len,
                    double const*restrict src,
                    long src_len1,
                    long src_len2,
                    long k1,
                    long k2,
                    double*restrict wrk);

extern void
tpl_filter_2d_2nd_d(double*restrict dst,
                    long dst_len1,
                    long dst_len2,
                    double const*restrict ker,
                    long ker_len,
                    double const*restrict src,
                    long src_len1,
                    long src_len2,
                    long k1,
                    long k2,
                    double*restrict wrk1,
                    double*restrict wrk2);

extern void
tpl_filter_2d_x1_1st_d(double*restrict dst,
                      long dst_len1,
                      long dst_len2,
                      double const*restrict ker,
                      double const*restrict src,
                      long src_len1,
                      long src_len2,
                      long k1,
                      long k2,
                      double*restrict wrk);

extern void
tpl_filter_2d_x1_2nd_d(double*restrict dst,
                      long dst_len1,
                      long dst_len2,
                      double const*restrict ker,
                      double const*restrict src,
                      long src_len1,
                      long src_len2,
                      long k1,
                      long k2,
                      double*restrict wrk1,
                      double*restrict wrk2);

extern void
tpl_filter_2d_x2_1st_d(double*restrict dst,
                      long dst_len1,
                      long dst_len2,
                      double const*restrict ker,
                      double const*restrict src,
                      long src_len1,
                      long src_len2,
                      long k1,
                      long k2,
                      double*restrict wrk);

extern void
tpl_filter_2d_x2_2nd_d(double*restrict dst,
                      long dst_len1,
                      long dst_len2,
                      double const*restrict ker,
                      double const*restrict src,
                      long src_len1,
                      long src_len2,
                      long k1,
                      long k2,
                      double*restrict wrk1,
                      double*restrict wrk2);

extern void
tpl_filter_2d_x3_1st_d(double*restrict dst,
                      long dst_len1,
                      long dst_len2,
                      double const*restrict ker,
                      double const*restrict src,
                      long src_len1,
                      long src_len2,
                      long k1,
                      long k2,
                      double*restrict wrk);

extern void
tpl_filter_2d_x3_2nd_d(double*restrict dst,
                      long dst_len1,
                      long dst_len2,
                      double const*restrict ker,
                      double const*restrict src,
                      long src_len1,
                      long src_len2,
                      long k1,
                      long k2,
                      double*restrict wrk1,
                      double*restrict wrk2);

extern void
tpl_filter_2d_x4_1st_d(double*restrict dst,
                      long dst_len1,
                      long dst_len2,
                      double const*restrict ker,
                      double const*restrict src,
                      long src_len1,
                      long src_len2,
                      long k1,
                      long k2,
                      double*restrict wrk);

extern void
tpl_filter_2d_x4_2nd_d(double*restrict dst,
                      long dst_len1,
                      long dst_len2,
                      double const*restrict ker,
                      double const*restrict src,
                      long src_len1,
                      long src_len2,
                      long k1,
                      long k2,
                      double*restrict wrk1,
                      double*restrict wrk2);

extern void
tpl_filter_2d_x5_1st_d(double*restrict dst,
                      long dst_len1,
                      long dst_len2,
                      double const*restrict ker,
                      double const*restrict src,
                      long src_len1,
                      long src_len2,
                      long k1,
                      long k2,
                      double*restrict wrk);

extern void
tpl_filter_2d_x5_2nd_d(double*restrict dst,
                      long dst_len1,
                      long dst_len2,
                      double const*restrict ker,
                      double const*restrict src,
                      long src_len1,
                      long src_len2,
                      long k1,
                      long k2,
                      double*restrict wrk1,
                      double*restrict wrk2);

_PVC_EXTERN_C_END

#endif /* _TPL_IMAGE_H */

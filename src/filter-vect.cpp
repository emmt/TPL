/*
 * filter-vect.cpp -
 *
 * Vectorized code for small compact stationary linear filters.
 *
 *-----------------------------------------------------------------------------
 *
 * This file is part of TPL software released under the MIT "Expat" license.
 *
 * Copyright (c) 2020: Éric Thiébaut <https://github.com/emmt/TPL>
 *
 */

#ifndef _TPL_FILTER_VCL_C
#define _TPL_FILTER_VCL_C 1

#ifndef USE_VCL
#  define USE_VCL 0
#endif

#if USE_VCL
#  include <vectorclass.h>
#endif
#include "tpl-filter.h"

/*
 * Integer type for indexing.
 */
#define _tpl_index long

/*
 * Best sizes for packed vectors of single and double precision
 * floating-points.
 */
#if USE_VCL
#  if defined(__AVX512F__)
#    define VECTOR_SIZE_FLOAT 16
#  elif defined(__AVX__)
#    define VECTOR_SIZE_FLOAT  8
#  elif defined(__SSE__)
#    define VECTOR_SIZE_FLOAT  4
#  else
#    define VECTOR_SIZE_FLOAT  1
#  endif
#  if defined(__AVX512F__)
#    define VECTOR_SIZE_DOUBLE 8
#  elif defined(__AVX__)
#    define VECTOR_SIZE_DOUBLE 4
#  elif defined(__SSE2__)
#    define VECTOR_SIZE_DOUBLE 2
#  else
#    define VECTOR_SIZE_DOUBLE 1
#  endif
#else /* do not vectorize code */
#  define VECTOR_SIZE_FLOAT  1
#  define VECTOR_SIZE_DOUBLE 1
#endif

#define ROUND_DOWN(a,b)    (((a)/(b))*(b))

#define _CAT2(_1,_2) _1##_2
#define CAT2(_1,_2) _CAT2(_1,_2)

#define _CAT3(_1,_2,_3) _1##_2##_3
#define CAT3(_1,_2,_3) _CAT3(_1,_2,_3)

#define VEC(siz,sfx) CAT3(Vec,siz,sfx)

#define CAT(x, ...) x ## __VA_ARGS__

/*
 * When a macro function gets expanded, expansion of the macro itself is
 * disabled (to avoid infinite loops).  This prevents writing simple
 * recurrences that could have simplified a lot most macro definitions.
 *
 * However see https://github.com/pfultz2/Cloak for ideas and workarounds.
 */

/*
 * `LIST(n,pfx)` expands as a list of `n` terms with prefix `pfx`.  The
 * result has the form `pfx0,pfx1,...`.
 */
#define LIST(n,pfx)   CAT(_LIST_,n)(pfx)

#define _LIST_1(pfx)                 CAT(pfx,0)
#define _LIST_2(pfx)   _LIST_1(pfx), CAT(pfx,1)
#define _LIST_3(pfx)   _LIST_2(pfx), CAT(pfx,2)
#define _LIST_4(pfx)   _LIST_3(pfx), CAT(pfx,3)
#define _LIST_5(pfx)   _LIST_4(pfx), CAT(pfx,4)

/*
 * Load filter coefficients.
 *
 * The call `LOAD_COEFS(n,T,pfx,arr)` expnads as:
 *
 * ```.c
 * T pfx0 = arr[0], pfx1 = arr[1], ...;
 * ```
 *
 * for the `n` coefficients of the filter stored in array `arr`.  Argument
 * `T` is the type of the variables to declare and initialize, `pfx` is the
 * prefix to use for naming these variables.
 */
#define LOAD_COEFS(n,T,pfx,arr)   T CAT(_LOAD_COEFS_,n)(pfx,arr)

#define _LOAD_COEFS_1(pfx,arr)                           CAT(pfx,0) = (arr)[0]
#define _LOAD_COEFS_2(pfx,arr)   _LOAD_COEFS_1(pfx,arr), CAT(pfx,1) = (arr)[1]
#define _LOAD_COEFS_3(pfx,arr)   _LOAD_COEFS_2(pfx,arr), CAT(pfx,2) = (arr)[2]
#define _LOAD_COEFS_4(pfx,arr)   _LOAD_COEFS_3(pfx,arr), CAT(pfx,3) = (arr)[3]
#define _LOAD_COEFS_5(pfx,arr)   _LOAD_COEFS_4(pfx,arr), CAT(pfx,4) = (arr)[4]

/*
 * Full loading of a group of packed values.
 *
 * The call `LOAD_FULL(n,pfx,arr,i)` expands as:
 *
 * ```.c
 * pfx0.load(&arr[i]);
 * pfx1.load(&arr[i+1]);
 * ...
 * ```
 *
 * for the `n` variables whose names are prefixed by `pfx`.
 */
#define LOAD_FULL(n,pfx,arr,i)   CAT(_LOAD_FULL_,n)(pfx,arr,i)

#define _LOAD_FULL_1(pfx,arr,i)                            CAT(pfx,0).load(&(arr)[(i)])
#define _LOAD_FULL_2(pfx,arr,i)   _LOAD_FULL_1(pfx,arr,i); CAT(pfx,1).load(&(arr)[(i)+1])
#define _LOAD_FULL_3(pfx,arr,i)   _LOAD_FULL_2(pfx,arr,i); CAT(pfx,2).load(&(arr)[(i)+2])
#define _LOAD_FULL_4(pfx,arr,i)   _LOAD_FULL_3(pfx,arr,i); CAT(pfx,3).load(&(arr)[(i)+3])
#define _LOAD_FULL_5(pfx,arr,i)   _LOAD_FULL_4(pfx,arr,i); CAT(pfx,4).load(&(arr)[(i)+4])

/*
 * Partial loading of a group of packed values.
 *
 * The call `LOAD_PART(n,pfx,arr,i)` expands as:
 *
 * ```.c
 * pfx0.load_partial(p, &arr[i]);
 * pfx1.load_partial(p, &arr[i+1]);
 * ...
 * ```
 *
 * for the `n` variables whose names are prefixed by `pfx`.
 */
#define LOAD_PART(n,p,pfx,arr,i)   CAT(_LOAD_PART_,n)(p,pfx,arr,i)

#define _LOAD_PART_1(p,pfx,arr,i)                              CAT(pfx,0).load_partial(p,&(arr)[(i)  ])
#define _LOAD_PART_2(p,pfx,arr,i)   _LOAD_PART_1(p,pfx,arr,i); CAT(pfx,1).load_partial(p,&(arr)[(i)+1])
#define _LOAD_PART_3(p,pfx,arr,i)   _LOAD_PART_2(p,pfx,arr,i); CAT(pfx,2).load_partial(p,&(arr)[(i)+2])
#define _LOAD_PART_4(p,pfx,arr,i)   _LOAD_PART_3(p,pfx,arr,i); CAT(pfx,3).load_partial(p,&(arr)[(i)+3])
#define _LOAD_PART_5(p,pfx,arr,i)   _LOAD_PART_4(p,pfx,arr,i); CAT(pfx,4).load_partial(p,&(arr)[(i)+4])

/*
 * Non-vectorized version of the filter.
 *
 * Argument `n` is the filter size, variables (w0,w1,...) contain the
 * filter coefficients, argument `a` is the source array and argument `i`
 * is the current index in the destination.  The macro expands as an
 * expression which can be used as an R-value.
 *
 */
#define FILTER(n,w,a,i)   CAT(_FILTER_,n)(w,a,i)

#define _FILTER_1(w,a,i)  ((w##0)*(a)[i])
#define _FILTER_2(w,a,i)  ((w##0)*(a)[i] + (w##1)*(a)[(i)+1])
#define _FILTER_3(w,a,i)  ((w##0)*(a)[i] + (w##1)*(a)[(i)+1] + \
                           (w##2)*(a)[(i)+2])
#define _FILTER_4(w,a,i)  (((w##0)*(a)[i]     + (w##1)*(a)[(i)+1]) + \
                           ((w##2)*(a)[(i)+2] + (w##3)*(a)[(i)+3]))
#define _FILTER_5(w,a,i)  (((w##0)*(a)[i]     + (w##1)*(a)[(i)+1]) + \
                           ((w##2)*(a)[(i)+2] + (w##3)*(a)[(i)+3]) + \
                           ((w##4)*(a)[(i)+4]))
/*
 * Apply a vectorized version of the filter.
 *
 * Variables (w0,w1,...) contain the filter coefficients.  Variables
 * (a0,a1,...) contain the packed values from the source at offsets
 * i+(0,1,...).  Variables (a0,a1,...) may be modified.  The result is left
 * in variable `r`.
*/
#define APPLY_FILTER(r,n,w,a)                   \
    do { CAT(_APPLY_FILTER_,n)(r,w,a); } while (0)

#define _APPLY_FILTER_1(r,w,a) r = a##0*w##0

#if defined(__FMA__)
#  define _APPLY_FILTER_2(r,w,a)                \
    r = a##0*w##0;                              \
    mul_add(a##1,w##1, r)
#  define _APPLY_FILTER_3(r,w,a)                \
    r = a##0*w##0;                              \
    r = mul_add(a##1,w##1, r);                  \
    r = mul_add(a##2,w##2, r)
#  define _APPLY_FILTER_4(r,ww,a)               \
    r = a##0*w##0;                              \
    r = mul_add(a##1,w##1, r);                  \
    r = mul_add(a##2,w##2, r);                  \
    r = mul_add(a##3,w##3, r)
#  define _APPLY_FILTER_5(r,w,a)                \
    r = a##0*w##0;                              \
    r = mul_add(a##1,w##1, r);                  \
    r = mul_add(a##2,w##2, r);                  \
    r = mul_add(a##3,w##3, r);                  \
    r = mul_add(a##4,w##4, r)
#else
#  define _APPLY_FILTER_2(r,w,a)                \
    a##0 *= w##0;                               \
    a##1 *= w##1;                               \
    r = a##0 + a##1
#  define _APPLY_FILTER_3(r,w,a)                \
    a##0 *= w##0;                               \
    a##1 *= w##1;                               \
    a##2 *= w##2;                               \
    r = a##0 + a##1 + a##2
#  define _APPLY_FILTER_4(r,w,a)                \
    a##0 *= w##0;                               \
    a##1 *= w##1;                               \
    a##2 *= w##2;                               \
    a##3 *= w##3;                               \
    a##0 += a##1;                               \
    a##2 += a##3;                               \
    r = a##0 + a##2
#  define _APPLY_FILTER_5(r,w,a)                \
    a##0 *= w##0;                               \
    a##1 *= w##1;                               \
    a##2 *= w##2;                               \
    a##3 *= w##3;                               \
    a##4 *= w##4;                               \
    a##0 += a##1;                               \
    a##2 += a##3;                               \
    r = a##0 + a##2 + a##4
#endif

/*
 * Generate code for single precision floating-point.
 */

#define _tpl_float      float
#define _tpl_func(name) CAT3(tpl_,name,_f)
#define _tpl_size       VECTOR_SIZE_FLOAT
#define _tpl_vect       CAT3(Vec,_tpl_size,f)

#define _tpl_kersiz 1
#include __FILE__
#undef _tpl_kersiz

#define _tpl_kersiz 2
#include __FILE__
#undef _tpl_kersiz

#define _tpl_kersiz 3
#include __FILE__
#undef _tpl_kersiz

#define _tpl_kersiz 4
#include __FILE__
#undef _tpl_kersiz

#define _tpl_kersiz 5
#include __FILE__
#undef _tpl_kersiz

#undef _tpl_float
#undef _tpl_func
#undef _tpl_size
#undef _tpl_vect


/*
 * Generate code for double precision floating-point.
 */

#define _tpl_float      double
#define _tpl_func(name) CAT3(tpl_,name,_d)
#define _tpl_size       VECTOR_SIZE_DOUBLE
#define _tpl_vect       CAT3(Vec,_tpl_size,d)

#define _tpl_kersiz 1
#include __FILE__
#undef _tpl_kersiz

#define _tpl_kersiz 2
#include __FILE__
#undef _tpl_kersiz

#define _tpl_kersiz 3
#include __FILE__
#undef _tpl_kersiz

#define _tpl_kersiz 4
#include __FILE__
#undef _tpl_kersiz

#define _tpl_kersiz 5
#include __FILE__
#undef _tpl_kersiz

#else /* _TPL_FILTER_VCL_C */

/*
 * Once all the macros have been defined, the code is deceptively small ;-)
 */
extern "C" void
_tpl_func(CAT2(filter_x,_tpl_kersiz))(_tpl_index n,
                                      _tpl_float *restrict dst,
                                      _tpl_float const*restrict ker,
                                      _tpl_float const*restrict src)
{
#if _tpl_size > 1
    _tpl_vect r, LIST(_tpl_kersiz, a);
    LOAD_COEFS(_tpl_kersiz, _tpl_vect, w, ker);
    _tpl_index m = ROUND_DOWN(n, _tpl_size);
    for (_tpl_index i = 0; i < m; i += _tpl_size) {
        LOAD_FULL(_tpl_kersiz, a, src, i);
        APPLY_FILTER(r, _tpl_kersiz, w, a);
        r.store(&dst[i]);
    }
    if (m < n) {
        int p = n - m;
        LOAD_PART(_tpl_kersiz, p, a, src, m);
        APPLY_FILTER(r, _tpl_kersiz, w, a);
        r.store_partial(p, &dst[m]);
    }
#else /* non-vectorized code */
    LOAD_COEFS(_tpl_kersiz, _tpl_float, w, ker);
    for (_tpl_index i = 0; i < n; ++i) {
        dst[i] = FILTER(_tpl_kersiz, w, src, i);
    }
#endif
}

#endif /* _TPL_FILTER_VCL_C */

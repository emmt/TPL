/*
 * tpl-interp.h -
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

#ifndef _TPL_INTERP_H
#define _TPL_INTERP_H 1

#include <tpl-base.h>

_TPL_EXTERN_C_BEGIN

/**
 * Opaque structure for any interpolation function.
 */
typedef struct TPL_InterpolationFunction TPL_InterpolationFunction;

/**
 * @def TPL_INTERP_FUNC(ptr,x)
 *
 * @brief Evaluate an interpolation function.
 *
 * This macro expands as an expression which yields the value of the
 * interpolation function pointed by `ptr` at coordinate `x`.  Argument `ptr`
 * is evaluated more than once.
 */
#define TPL_INTERP_FUNC(ptr, x) \
    ((ptr)->func((TPL_InterpolationFunction const*)ptr, x))
/**
 * @def TPL_INTERP_DERIV(ptr,x)
 *
 * @brief Evaluate the derivative of an interpolation function.
 *
 * This macro expands as an expression which yields the value of the derivative
 * of the interpolation function pointed by `ptr` at coordinate `x`.  Argument
 * `ptr` is evaluated more than once.
 */
#define TPL_INTERP_DERIV(ptr, x) \
    ((ptr)->deriv((TPL_InterpolationFunction const*)ptr, x))

/**
 * @def TPL_INTERP_FUNC_WGTS(ptr,t,w)
 *
 * @brief Compute interpolation weights of an interpolation function.
 *
 * This macro stores in `w` the interpolation weights for the interpolation
 * function pointed by `ptr` at offset `t`.  Argument `ptr` is evaluated more
 * than once.
 */
#define TPL_INTERP_FUNC_WGTS(ptr, t, w) \
    ((ptr)->func_wgts((TPL_InterpolationFunction const*)ptr, t, w))
/**
 * @def TPL_INTERP_DERIV_WGTS(ptr, t, w)
 *
 * @brief Compute interpolation weights of the derivative of an interpolation
 * function.
 *
 * This macro stores in `w` the interpolation weights for the derivative of the
 * interpolation function pointed by `ptr` at offset `t`.  Argument `ptr` is
 * evaluated more than once.
 */
#define TPL_INTERP_DERIV_WGTS(ptr, x) \
    ((ptr)->deriv_wgts((TPL_InterpolationFunction const*)ptr, t, w))

/**
 * Opaque structure for a cardinal cubic spline interpolation function.
 */
typedef struct TPL_CardinalCubicSpline TPL_CardinalCubicSpline;

/**
 * @def _TPL_INTERPOLATION_FUNCTION
 *
 * This macros expands to the leading fields of any structure implementing an
 * interpolation function.
 */
#define _TPL_INTERPOLATION_FUNCTION                                       \
    long size;                                                            \
    double (*func)(TPL_InterpolationFunction const*, double);             \
    void (*func_wgts)(TPL_InterpolationFunction const*, double, double*); \
    double (*deriv)(TPL_InterpolationFunction const*, double);            \
    void (*deriv_wgts)(TPL_InterpolationFunction const*, double, double*)

/**
 * Structure for a cardinal cubic spline interpolation function.
 *
 * This structure shall be initialized by tpl_init_cardinal_cubic_spline().
 */
struct TPL_CardinalCubicSpline {
    _TPL_INTERPOLATION_FUNCTION;
    double c;
    double f1, f2, f3;
    double d1, d2, d3, d4;
};

/**
 * Initialize a cardinal cubic spline.
 *
 * A cardinal cubic spline is a piecewise cubic interpolation function whose
 * support has size 4 and which depends one a *tension* parameter `c`.
 *
 * The slope at `x = ±1` is `±(c - 1)/2`.  Usually `c ≤ 1`, choosing `c = 0`
 * yields a Catmull-Rom spline, `c = 1` yields all zero tangents, `c = -1`
 * yields a truncated approximation of a cardinal sine, `c = -1/2` yields an
 * interpolating cubic spline with continuous second derivatives (inside its
 * support).
 *
 * @param c  The tension parameter.
 *
 * After initialization, a cardinal cubic spline structure can be used to
 * evaluate the function or its derivative.  For instance:
 *
 * ```.c
 * TPL_CardinalCubicSpline phi;
 * tpl_initialize_cardinal_cubic_spline(&phi, 0.5);
 * double x = 1.23;
 * double fx = TPL_INTERP_FUNC(&phi, x);  // yields phi(x)
 * double dx = TPL_INTERP_DERIV(&phi, x); // yields phi'(x)
 * ```
 */
extern void
tpl_initialize_cardinal_cubic_spline(TPL_CardinalCubicSpline* ker, double c);

_TPL_EXTERN_C_END

#endif /* _TPL_INTERP_H */

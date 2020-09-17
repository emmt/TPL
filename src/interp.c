/*
 * interp.c -
 *
 * Implementation of interpolation functions.
 *
 *-----------------------------------------------------------------------------
 *
 * This file is part of TPL software released under the MIT "Expat" license.
 *
 * Copyright (c) 2020: Éric Thiébaut <https://github.com/emmt/TPL>
 *
 */

#include "tpl-interp.h"
#include <math.h>

/* This is just to have a concrete definition. */
struct TPL_InterpolationFunction {
     _TPL_INTERPOLATION_FUNCTION;
};

#define frac(T,a,b)  (((T)(a))/((T)(b)))

#define square(x)                               \
    _Generic(x,                                 \
             float:       square_d,             \
             long double: square_ld,            \
             default:     square_d)(x)

#define cube(x)                                 \
    _Generic(x,                                 \
             float:       cube_d,               \
             long double: cube_ld,              \
             default:     cube_d)(x)

#define ENCODE(T,sfx)                           \
    static inline T square_##sfx(T _tpl_x)      \
    {                                           \
        return _tpl_x*_tpl_x;                   \
    }                                           \
                                                \
    static inline T cube_##sfx(T _tpl_x)        \
    {                                           \
        return _tpl_x*_tpl_x*_tpl_x;            \
    }                                           \

ENCODE(float,       f)
ENCODE(double,      d)
ENCODE(long double, ld)

#undef ENCODE

// Cardinal cubic spline function.
static double
cardinal_cubic_spline_func(TPL_InterpolationFunction const* ptr, double x)
{
    TPL_CardinalCubicSpline const* obj = (TPL_CardinalCubicSpline const*)ptr;
    double ax = fabs(x);
    if (ax >= 2.0) {
        return 0.0;
    } else if (ax >= 1.0) {
        return obj->f1*(ax - 1.0)*square(2 - ax);
    } else {
        return ((obj->f3*ax - 1)*ax - 1)*(ax - 1);
    }
}

static void
cardinal_cubic_spline_func_weights(TPL_InterpolationFunction const* ptr,
                                   double t, double* w)
{
    TPL_CardinalCubicSpline const* obj = (TPL_CardinalCubicSpline const*)ptr;
    /*
     * Computation of:
     *     w1 = f1 t u²
     *     w2 = u + t u² - f2 t² u
     *     w3 = t + t² u - f2 t u²
     *     w4 = f1 t² u
     * with u = 1 - t in 13 operations.
     */
    double u = 1 - t;
    double tu = t*u;
    double ptu = obj->f1*tu;
    w[0] = ptu*u;
    w[1] = (u - obj->f2*t)*tu + u;
    w[2] = (t - obj->f2*u)*tu + t;
    w[3] = ptu*t;
}

// First derivative of the cardinal cubic spline.
static double
cardinal_cubic_spline_deriv(TPL_InterpolationFunction const* ptr, double x)
{
    TPL_CardinalCubicSpline const* obj = (TPL_CardinalCubicSpline const*)ptr;
    if (x < 0) {
        if (x <= -2) {
            return 0;
        } else if (x < -1) {
            return -(x + 2)*(x + frac(double,4,3))*obj->d1;
        } else {
            return -(x + obj->d3)*x*obj->d2;
        }
    } else {
        if (x >= 2) {
            return 0;
        } else if (x > 1) {
            return (x - 2)*(x - frac(double,4,3))*obj->d1;
        } else {
            return (x - obj->d3)*x*obj->d2;
        }
    }
}

static void
cardinal_cubic_spline_deriv_weights(TPL_InterpolationFunction const* ptr,
                                    double t, double* w)
{
    TPL_CardinalCubicSpline const* obj = (TPL_CardinalCubicSpline const*)ptr;
    /*
     * Computation of:
     *     w1 = d1*(t - 1)*(t - 1/3)
     *     w2 = d2*(t - d3)*t
     *     w3 = d2*(t - 1)*(d4 - t)
     *     w4 = d1*t*(2/3 - t)
     * in 13 operations.
     */
    double u = t - 1;
    w[0] = obj->d1*u*(t - frac(double,1,3));
    w[1] = obj->d2*(t - obj->d3)*t;
    w[2] = obj->d2*u*(obj->d4 - t);
    w[3] = obj->d1*t*(frac(double,2,3) - t);
}

void
tpl_init_cardinal_cubic_spline(TPL_CardinalCubicSpline* obj, double c)
{
    double q = (c + 1.0)/2.0;
    double t = 3*c + 9;
    obj->c = c;
    obj->f1 = q - 1;
    obj->f2 = q;
    obj->f3 = q + 1;
    obj->d1 = (3*c - 3)/2;
    obj->d2 = t/2;
    obj->d3 = (2*c + 10)/t;
    obj->d4 = (c - 1)/t;
    obj->size = 4;
    obj->func = cardinal_cubic_spline_func;
    obj->func_wgts = cardinal_cubic_spline_func_weights;
    obj->deriv = cardinal_cubic_spline_deriv;
    obj->deriv_wgts = cardinal_cubic_spline_deriv_weights;
}

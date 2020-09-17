/*
 * interp-tests.c -
 *
 * Testing interpolation functions.
 *
 *-----------------------------------------------------------------------------
 *
 * This file is part of TPL software released under the MIT "Expat" license.
 *
 * Copyright (c) 2020: Éric Thiébaut <https://github.com/emmt/TPL>
 *
 */

#include <stdlib.h> /* for EXIT_SUCCESS, etc. */
#include <stdio.h>
#include <math.h>
#include <pvc-math.h>
#include "tpl-interp.h"

int main(int argc, char* argv[])
{
    int status = EXIT_SUCCESS;
    long cnt = 0;
    double eps = 1e-7;
    double sum1 = 0.0;
    double sum2 = 0.0;
    double err_max = 0.0;
    double err_min = PVC_TYPE_MAX(double);
    TPL_CardinalCubicSpline phi;
    tpl_initialize_cardinal_cubic_spline(&phi, 0.0);
    for (double x = -2.01; x <= 2.1; x += 0.0003) {
        double dx = TPL_INTERP_DERIV(&phi, x); // exact phi'(x)
        double f1 = TPL_INTERP_FUNC(&phi, x - eps);
        double f2 = TPL_INTERP_FUNC(&phi, x + eps);
        double de = (f2 - f1)/(2*eps); // estimated derivative
        double r = dx - de;
        sum1 += r;
        sum2 += r*r;
        double e = fabs(r);
        err_max = pvc_max(err_max, e);
        err_min = pvc_min(err_min, e);
        ++cnt;
    }
    printf("number of samples = %ld\n", cnt);
    printf("abs. err. min. = %g\n", err_min);
    printf("abs. err. max. = %g\n", err_max);
    double err_avg = sum1/cnt;
    double err_std = sqrt((sum2 - (sum1/cnt)*sum1)/(cnt - 1));
    printf("err. = %g +/- %g\n", err_avg, err_std);
    return status;
}

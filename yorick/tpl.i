/*
 * tpl.i -
 *
 * Interface to the TPL library.
 *
 *-----------------------------------------------------------------------------
 *
 * This file is part of TPL software released under the MIT "Expat" license.
 *
 * Copyright (c) 2020: Éric Thiébaut et al. <https://github.com/emmt/TPL>
 */

if (is_func(plug_in)) plug_in, "yor_tpl";

extern _tpl_filter_2d_ref_f;
/* PROTOTYPE void tpl_filter_2d_ref_f(int dim,
                                      float array dst,
                                      long dst_len1,
                                      long dst_len2,
                                      float array ker,
                                      long ker_len,
                                      float array src,
                                      long src_len1,
                                      long src_len2,
                                      long k1,
                                      long k2,
                                      float array wrk1,
                                      float array wrk2); */

extern _tpl_filter_2d_ref_d;
/* PROTOTYPE void tpl_filter_2d_ref_d(int dim,
                                      double array dst,
                                      long dst_len1,
                                      long dst_len2,
                                      double array ker,
                                      long ker_len,
                                      double array src,
                                      long src_len1,
                                      long src_len2,
                                      long k1,
                                      long k2,
                                      double array wrk1,
                                      double array wrk2); */

func tpl_cardinal_cubic_spline(x, tension=, deriv=)
/* DOCUMENT tpl_cardinal_cubic_spline(x)

     Get the value of the cardinal cubic-spline at `x`.

     Keyword `tension` (default value 0) can be used to set the tension of
     the cardinal cubic-spline.  The slope at `x = ±1` is `±(t - 1)/2` where
     `t` is the tension parameter.  Usually `t ≤ 1`, choosing `t = 0` yields a
     Catmull-Rom spline, `t = -1` yields a truncated approximation of a
     cardinal sine and `t = -1/2` yields an interpolating cubic spline with
     continuous second derivatives (inside its support).

     Keyword `deriv` (default false) can be set true to yield the derivative of
     the function instead.

   SEE ALSO:
 */
{
    c = (is_void(tension) ? 0.0 : double(tension));
    y = array(double, dimsof(x));
    ax = abs(x);
    i = where(ax < 2.0);
    if (is_array(i)) {
        ax = double(ax(i));
        big = (ax > 1.0);
        j1 = where(!big);
        j2 = where(big);
        if (deriv) {
            s = double(sign(x(i)));
            t = 3.0*c + 9.0;
            d1 = (3.0*c - 3.0)/2.0;
            d2 = t/2.0;
            d3 = (2.0*c + 10.0)/t;
            d4 = (c - 1.0)/t;
            if (is_array(j1)) {
                z = ax(j1);
                y(i(j1)) = s(j1)*(z - d3)*z*d2;
            }
            if (is_array(j2)) {
                z = ax(j2);
                y(i(j2)) = s(j2)*(z - 2.0)*(z - (4.0/3.0))*d1;
            }
        } else {
            f2 = (c + 1.0)/2.0;
            f1 = f2 - 1.0;
            f3 = f2 + 1.0;
            if (is_array(j1)) {
                z = ax(j1);
                y(i(j1)) = ((f3*z - 1.0)*z - 1.0)*(z - 1.0);
            }
            if (is_array(j2)) {
                z = ax(j2);
                t = 2.0 - z;
                y(i(j2)) = f1*(z - 1.0)*t*t;
            }
        }
    }
    return y;
}

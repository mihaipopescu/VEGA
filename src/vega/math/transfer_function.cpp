#include "assert.h"
#include "transfer_function.h"


using namespace vega::math;


vega::math::transfer_function::transfer_function(const std::vector<transfer_control_point>& vPaletteKnots)
{
    vector4d transferFunc[256];

    assert(vPaletteKnots.size() == 256);

    for (int i = 0; i < 256; i++)
    {
        vector4d& color = vPaletteKnots[i].Color * 255.f;
        myTransferArray[i] = r8g8b8a8((uint8)color.x, (uint8)color.y, (uint8)color.z, (uint8)color.w);
    }
}

vega::math::transfer_function::transfer_function(const std::vector<transfer_control_point>& vColorKnots, const std::vector<transfer_control_point>& vAlphaKnots)
{
    //initialize the cubic spline for the transfer function
    vector4d transferFunc[256];

    int cn = vColorKnots.size();
    int an = vAlphaKnots.size();

    std::vector<cubic4d> colorCubic;
    compute_cubic_4d_spline(vColorKnots, colorCubic);

    assert(colorCubic.size() == cn - 1);

    int i;
    int numTF = 0;
    for (i = 0; i < cn - 1; i++)
    {
        int steps = vColorKnots[i + 1].IsoValue - vColorKnots[i].IsoValue;

        for (int j = 0; j < steps; j++)
        {
            float k = (float)j / (float)steps;

            transferFunc[numTF++] = colorCubic[i].evaluate(k);
        }
    }

    std::vector<cubic4d> alphaCubic;
    compute_cubic_4d_spline(vAlphaKnots, alphaCubic);

    assert(alphaCubic.size() == an - 1);

    numTF = 0;
    for (i = 0; i < an - 1; i++)
    {
        int steps = vAlphaKnots[i + 1].IsoValue - vAlphaKnots[i].IsoValue;

        for (int j = 0; j < steps; j++)
        {
            float k = (float)j / (float)steps;

            transferFunc[numTF++].w = alphaCubic[i].evaluate(k).w;
        }
    }

    for (i = 0; i < 256; i++)
    {
        vector4d& color = transferFunc[i] * 255.0f;

        if( color.w < 0.f )
            color.w = 0.f;

        myTransferArray[i] = r8g8b8a8((uint8)color.x, (uint8)color.y, (uint8)color.z, (uint8)color.w);
    }
}

void vega::math::transfer_function::compute_cubic_4d_spline(const std::vector<transfer_control_point> &vControlPoints, std::vector<cubic4d>& cubic4dspline) const
{
    int i, n = vControlPoints.size() - 1;
    vector4d *gamma = new vector4d[n+1];
    vector4d *delta = new vector4d[n+1];
    vector4d *D     = new vector4d[n+1];

    /* We need to solve the equation
    * taken from: http://mathworld.wolfram.com/CubicSpline.html
    [2 1       ] [D[0]]   [3(v[1] - v[0])  ]
    |1 4 1     | |D[1]|   |3(v[2] - v[0])  |
    |  1 4 1   | | .  | = |      .         |
    |    ..... | | .  |   |      .         |
    |     1 4 1| | .  |   |3(v[n] - v[n-2])|
    [       1 2] [D[n]]   [3(v[n] - v[n-1])]

    by decomposing the matrix into upper triangular and lower matrices
    and then back substitution.  See Spath "Spline Algorithms for Curves
    and Surfaces" pp 19--21. The D[i] are the derivatives at the knots.
    */

    //this builds the coefficients of the left matrix
    gamma[0].x = 1.f / 2.f;
    gamma[0].y = 1.f / 2.f;
    gamma[0].z = 1.f / 2.f;
    gamma[0].w = 1.f / 2.f;

    for (i = 1; i < n; i++)
    {
        vector4d d(4.f, 4.f, 4.f, 4.f);
        d -= gamma[i-1];
        gamma[i] = vector4d(1.f/d.x, 1.f/d.y, 1.f/d.z, 1.f/d.w);
    }

    vector4d d(2.f, 2.f, 2.f, 2.f);
    d -= gamma[n-1];
    gamma[n] = vector4d(1.f/d.x, 1.f/d.y, 1.f/d.z, 1.f/d.w);

    delta[0] = (vControlPoints[1].Color - vControlPoints[0].Color) * 3;
    delta[0] = delta[0] * gamma[0];

    for (i = 1; i < n; i++)
    {
        delta[i] = (vControlPoints[i+1].Color - vControlPoints[i].Color) * 3 - delta[i-1];
        delta[i] = delta[i] * gamma[i];
    }
    delta[n] = (vControlPoints[n].Color - vControlPoints[n - 1].Color) * 3 - delta[n - 1];
    delta[n] = delta[n] * gamma[n];

    D[n] = delta[n];
    for (i = n - 1; i >= 0; i--)
    {
        D[i] = gamma[i] * D[i + 1];
        D[i] = delta[i] - D[i];
    }

    // now compute the coefficients of the cubics 
    for (i = 0; i < n; i++)
    {
        vector4d a = vControlPoints[i].Color;
        vector4d b = D[i];
        vector4d c = (vControlPoints[i + 1].Color - vControlPoints[i].Color)*3 - D[i]*2 - D[i + 1];
        vector4d d = (vControlPoints[i].Color - vControlPoints[i + 1].Color)*2 + D[i] + D[i + 1];

        cubic4dspline.push_back(cubic4d(a, b, c, d));
    }

    delete [] gamma;
    delete [] delta;
    delete [] D;
}

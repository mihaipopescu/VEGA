#include <xutility>
#include <algorithm>
#include "transformations.h"


void vega::math::RGBtoHSV( float r, float g, float b, float *h, float *s, float *v )
{
    float fMin, fMax, fDelta;

    fMin = std::min( r, std::min(g, b) );
    fMax = std::max( r, std::max(g, b) );
    *v = fMax;				// v

    fDelta = fMax - fMin;

    if( fDelta == 0 )
        *s = 0;
    else {
        *s = fDelta / *v;
    }

    if( fDelta == 0 )
        *h = 0; // hue is undefined but assigned to 0 for convenience
    else if( r == fMax )
        *h = ( g - b ) / fDelta;		// between yellow & magenta
    else if( g == fMax )
        *h = 2 + ( b - r ) / fDelta;	// between cyan & yellow
    else
        *h = 4 + ( r - g ) / fDelta;	// between magenta & cyan

    *h *= 60.f;				// degrees
    if( *h < 0 )
        *h += 360.f;

    *v /= 255.f;
    *h = GRD2RAD(*h);
}

void vega::math::HSVtoRGB( float h, float s, float v, float *r, float *g, float *b )
{
    float c = v*s;
    float x = c;
    float m = v - c;

    h = RAD2GRD(h);

    if( h >= 360.f )
        h = 0.f;
    if( h < 0.f )
        h += 360.f;

    int hh = (int)(h / 60.f);
    
    x *= 1 - (hh%2 - 1);

    switch(hh)
    {
    case 0:
        *r = c;
        *g = x;
        *b = 0.f;
        break;
    case 1:
        *r = x;
        *g = c;
        *b = 0.f;
        break;
    case 2:
        *r = 0;
        *g = c;
        *b = x;
        break;
    case 3:
        *r = 0;
        *g = x;
        *b = c;
        break;
    case 4:
        *r = x;
        *g = 0;
        *b = c;
        break;
    case 5:
        *r = c;
        *g = 0;
        *b = x;
        break;
    }

    *r += m;
    *g += m;
    *b += m;
 }

float vega::math::HSV_distance_from_RGB( const vector3d & c1, const vector3d & c2 )
{
    float h1, s1, v1;
    float h2, s2, v2;

    RGBtoHSV(c1.x, c1.y, c1.z, &h1, &s1, &v1);
    RGBtoHSV(c2.x, c2.y, c2.z, &h2, &s2, &v2);

    // distance in HSV space (ref: Smith et. al)
    //return 1.f - 1.f/sqrtf(5) * sqrtf( SQR(v1 - v2) + SQR(s1*cosf(h1) - s2*cosf(h2)) + SQR(s1*sinf(h1) - s2*sinf(h2)) );

    // Euclidean distance in HSV space
    vector3d hsv1(v1*s1*cosf(h1), v1*s1*sinf(h1), v1);
    vector3d hsv2(v2*s2*cosf(h2), v2*s2*sinf(h2), v2);
    
    return hsv1.dist(hsv2);
}

void vega::math::transform_coord_array( std::vector<vector3d>& vertices, const matrix4x4& m )
{
    int iIndex = 0;
    std::for_each(vertices.begin(), vertices.end(), [m, &vertices, &iIndex](vector3d& v)
    {
        vertices[iIndex++] = (vector3d)(transform_coord(m, v).homogenize());
    });
}

vega::math::matrix4x4 vega::math::make_inverse( const matrix4x4 &m )
{
    float p = 1.f;
    matrix4x4 out;
    vector4d v, vec[3];

    float det = matrix_determinant(m);
    if ( is_null(det) )
        return m;
    
    for (int i=0; i<4; i++)
    {
        for (int j=0; j<4; j++)
        {
            if (j != i )
            {
                int a = j;
                if ( j > i ) a = a-1;
                vec[a].x = m.m[j][0];
                vec[a].y = m.m[j][1];
                vec[a].z = m.m[j][2];
                vec[a].w = m.m[j][3];
            }
        }

        v = cross4d(vec[0], vec[1], vec[2]);
        out.m[0][i] = p * v.x / det;
        out.m[1][i] = p * v.y / det;
        out.m[2][i] = p * v.z / det;
        out.m[3][i] = p * v.w / det;

        p *= -1.f;
    }

    return out;
}

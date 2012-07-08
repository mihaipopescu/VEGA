#ifndef __VEGA_MATH_COMMON_H__
#define __VEGA_MATH_COMMON_H__

#include <math.h>

#define SQR(x) ((x)*(x))

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

namespace vega
{
    namespace math
    {
        const float PI = 3.1415926535897932384626433832795f;
        const float PIBY2 = 1.5707963267948966192313216916398f;

        const float EPSILON = 1e-6f;
        const float INF = (float)(0x80000000);

        inline bool is_null(float v)
        {
            return fabsf(v) < EPSILON;
        }
    }
}

#endif
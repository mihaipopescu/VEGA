#ifndef __VEGA_MATH_VECTOR4D_H__
#define __VEGA_MATH_VECTOR4D_H__

#include "vector3d.h"

namespace vega
{
    namespace math
    {

        class vector4d
        {
        public:
            vector4d(float _x = 0.f, float _y = 0.f, float _z = 0.f, float _w = 0.f) : x(_x), y(_y), z(_z), w(_w) {}

            vector4d operator+(const vector4d& v) const
            {
                return vector4d(x + v.x, y + v.y, z + v.z, w + v.w);
            }

            vector4d operator-(const vector4d& v) const
            {
                return vector4d(x - v.x, y - v.y, z - v.z, w - v.w);
            }

            vector4d operator*(float f) const
            {
                return vector4d(f * x, f * y, f * z, f * w);
            }
            
            vector4d operator*(const vector4d& v) const
            {
                return vector4d(x * v.x, y * v.y, z * v.z, w * v.w);
            }

            vector4d& operator-=(const vector4d& v)
            {
                x -= v.x;
                y -= v.y;
                z -= v.z;
                w -= v.w;
                return *this;
            }

            vector4d& operator+=(const vector4d& v)
            {
                x += v.x;
                y += v.y;
                z += v.z;
                w += v.w;
                return *this;
            }

            vector4d& homogenize()
            {
                x /= w;
                y /= w;
                z /= w;
                w = 1.f;
                return *this;
            }

            operator vector3d()
            {
                return vector3d(x, y, z);
            }

        public:
            float x, y, z, w;
        };
    }
}

#endif
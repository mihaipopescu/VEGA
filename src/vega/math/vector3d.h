#ifndef __VEGA_MATH_VECTOR3D_H__
#define __VEGA_MATH_VECTOR3D_H__

#include "common.h"
#include "vector2d.h"

namespace vega
{
    namespace math
    {

        class vector3d
        {
        public:
            vector3d(float _x = 0.f, float _y = 0.f, float _z = 0.f) : x(_x), y(_y), z(_z) {}

            vector3d operator+(const vector3d& v) const
            {
                return vector3d(x + v.x, y + v.y, z + v.z);
            }

            vector3d operator-() const
            {
                return vector3d(-x, -y, -z);
            }

            vector3d operator-(const vector3d& v) const
            {
                return vector3d(x - v.x, y - v.y, z - v.z);
            }

            vector3d& operator+=(const vector3d& v)
            {
                x += v.x;
                y += v.y;
                z += v.z;
                return *this;
            }

            vector3d& operator-=(const vector3d& v)
            {
                x -= v.x;
                y -= v.y;
                z -= v.z;
                return *this;
            }

            vector3d operator*(float f) const
            {
                return vector3d(x * f, y * f, z * f);
            }

            float dot(const vector3d& v) const
            {
                return x*v.x + y*v.y + z*v.z;
            }

            vector3d& cross(const vector3d& b)
            {
                float _x = y*b.z - z*b.y;
                float _y = z*b.x - x*b.z;
                float _z = x*b.y - y*b.x;

                x = _x;
                y = _y;
                z = _z;

                return *this;
            }

            float norm() const
            {
                return sqrtf(x*x + y*y + z*z);
            }

            float dist(const vector3d& p) const
            {
                float dx = x - p.x, dy = y - p.y, dz = z - p.z;
                return sqrtf(dx*dx + dy*dy + dz*dz);
            }

            vector3d& normalize()
            {
                float d = norm();
                x /= d;
                y /= d;
                z /= d;
                return *this;
            }

            vector3d& homogenize()
            {
                x /= z;
                y /= z;
                z = 1.f;
                return *this;
            }

            bool is_null() const
            {
                return fabsf(norm()) < EPSILON;
            }

            operator vector2d()
            {
                return vector2d(x, y);
            }

        public:
            float x, y, z;
        };

    }

}

#endif
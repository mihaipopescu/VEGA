#ifndef __VEGA_MATH_VECTOR2D_H__
#define __VEGA_MATH_VECTOR2D_H__

#include <math.h>

namespace vega
{
    namespace math
    {
        class vector2d
        {
        public:
            vector2d(float _x = 0.f, float _y = 0.f) : x(_x), y(_y) { }

            vector2d operator+(const vector2d& v) const
            {
                return vector2d(x + v.x, y + v.y);
            }

            float dot(const vector2d& v) const
            {
                return x*v.x + y*v.y;
            }

            vector2d& neg()
            {
                x = -x;
                y = -y;
                return *this;
            }

            float norm() const
            {
                return sqrtf(x*x + y*y);
            }

            float dist(const vector2d& p) const
            {
                float dx = x - p.x, dy = y - p.y;
                return sqrtf(dx*dx + dy*dy);
            }

        public:
            float x, y;
        };

    }
}

#endif
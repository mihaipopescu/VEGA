#ifndef __VEGA_MATH_PLANE_H__
#define __VEGA_MATH_PLANE_H__

#include "common.h"
#include "vector3d.h"


namespace vega
{
    namespace math
    {
        class plane
        {
        public:
            plane(float _a, float _b, float _c, float _d) : a(_a), b(_b), c(_c), d(_d) { }
            plane(const vector3d& point, const vector3d& normal)
            {
                a = normal.x;
                b = normal.y;
                c = normal.z;
                d = -point.dot(normal);
            }

            float dot_normal(const vector3d& v) const
            {
                return a*v.x + b*v.y + c*v.z;
            }

            float dot_coord(const vector3d& v) const
            {
                return a*v.x + b*v.y + c*v.z + d;
            }

            bool intersect_segment(const vector3d& p1, const vector3d& p2, vector3d& result) const
            {
                float t = dot_coord(p1);
                vector3d r = p1 - p2;
                float d = dot_normal(r);

                if( fabsf(d) < EPSILON )
                    return false;

                t /= d;

                if(t >= 0.f && t<=1.f)
                {
                    result.x = p1.x + (p2.x - p1.x)*t;
                    result.y = p1.y + (p2.y - p1.y)*t;
                    result.z = p1.z + (p2.z - p1.z)*t;
                    return true;
                }

                return false;
            }

        public:
            float a, b, c, d;
        };
    }
}

#endif
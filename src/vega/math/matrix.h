#ifndef __VEGA_MATH_MATRIX_H__
#define __VEGA_MATH_MATRIX_H__


namespace vega
{
    namespace math
    {
        class matrix4x4
        {
        public:
            union 
            {
                struct 
                { 
                    float _11, _12, _13, _14;
                    float _21, _22, _23, _24;
                    float _31, _32, _33, _34;
                    float _41, _42, _43, _44;
                };
                float m[4][4];
                float a[16];
            };
        };


        class matrix3x3
        {
        public:
            union 
            {
                struct 
                { 
                    float _11, _12, _13;
                    float _21, _22, _23;
                    float _31, _32, _33;
                };
                float m[3][3];
                float a[16];
            };
        };
    }
}

#endif
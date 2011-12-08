#ifndef __VEGA_MATH_TRANSFORMATIONS_H__
#define __VEGA_MATH_TRANSFORMATIONS_H__

#include "common.h"
#include "vector2d.h"
#include "vector3d.h"
#include "vector4d.h"
#include "matrix.h"
#include <math.h>
#include <vector>

#define swap3(t, a, b) {t c=a;a=b;b=c;}
#define swap2_begin(t) {t c;
#define swap2(a, b) c=a;a=b;b=c;
#define swap2_end() }

#define GRD2RAD(x) ((x) * 2*PI/360.f)
#define RAD2GRD(x) ((x) * 360.f/(2*PI))


namespace vega
{
    namespace math
    {
        //! 4D matrix multiplication
        inline matrix4x4 matrix_multiply(const matrix4x4& a, const matrix4x4& b)
        {
            matrix4x4 c;
            c._11 = a._11*b._11 + a._12*b._21 + a._13*b._31 + a._14*b._41;
            c._12 = a._11*b._12 + a._12*b._22 + a._13*b._32 + a._14*b._42;
            c._13 = a._11*b._13 + a._12*b._23 + a._13*b._33 + a._14*b._43;
            c._14 = a._11*b._14 + a._12*b._24 + a._13*b._34 + a._14*b._44;

            c._21 = a._21*b._11 + a._22*b._21 + a._23*b._31 + a._24*b._41;
            c._22 = a._21*b._12 + a._22*b._22 + a._23*b._32 + a._24*b._42;
            c._23 = a._21*b._13 + a._22*b._23 + a._23*b._33 + a._24*b._43;
            c._24 = a._21*b._14 + a._22*b._24 + a._23*b._34 + a._24*b._44;

            c._31 = a._31*b._11 + a._32*b._21 + a._33*b._31 + a._34*b._41;
            c._32 = a._31*b._12 + a._32*b._22 + a._33*b._32 + a._34*b._42;
            c._33 = a._31*b._13 + a._32*b._23 + a._33*b._33 + a._34*b._43;
            c._34 = a._31*b._14 + a._32*b._24 + a._33*b._34 + a._34*b._44;

            c._41 = a._41*b._11 + a._42*b._21 + a._43*b._31 + a._44*b._41;
            c._42 = a._41*b._12 + a._42*b._22 + a._43*b._32 + a._44*b._42;
            c._43 = a._41*b._13 + a._42*b._23 + a._43*b._33 + a._44*b._43;
            c._44 = a._41*b._14 + a._42*b._24 + a._43*b._34 + a._44*b._44;

            return c;
        }

        //! 3D matrix multiplication
        inline matrix3x3 matrix_multiply(const matrix3x3& a, const matrix3x3& b)
        {
            matrix3x3 c;
            c._11 = a._11*b._11 + a._12*b._21 + a._13*b._31;
            c._12 = a._11*b._12 + a._12*b._22 + a._13*b._32;
            c._13 = a._11*b._13 + a._12*b._23 + a._13*b._33;

            c._21 = a._21*b._11 + a._22*b._21 + a._23*b._31;
            c._22 = a._21*b._12 + a._22*b._22 + a._23*b._32;
            c._23 = a._21*b._13 + a._22*b._23 + a._23*b._33;

            c._31 = a._31*b._11 + a._32*b._21 + a._33*b._31;
            c._32 = a._31*b._12 + a._32*b._22 + a._33*b._32;
            c._33 = a._31*b._13 + a._32*b._23 + a._33*b._33;

            return c;
        }

        //! transform a coordinate with a row major matrix (post-multiplication)
        inline vector3d transform_coord(const matrix3x3& m, const vector2d& v)
        {
            vector3d result;
            result.x = m._11*v.x + m._12*v.y + m._13;
            result.y = m._21*v.x + m._22*v.y + m._23;
            result.z = m._31*v.x + m._32*v.y + m._33;
            return result;
        }

        //! transform coordinate with a row major matrix (post-multiplication)
        inline vector4d transform_coord(const matrix4x4& m, const vector3d& v)
        {
            vector4d result;
            result.x = m._11*v.x + m._12*v.y + m._13*v.z + m._14;
            result.y = m._21*v.x + m._22*v.y + m._23*v.z + m._24;
            result.z = m._31*v.x + m._32*v.y + m._33*v.z + m._34;
            result.w = m._41*v.x + m._42*v.y + m._43*v.z + m._44;
            return result;
        }

        //! make 4D identity matrix
        inline matrix4x4& make_identity(matrix4x4& m)
        {
            m._11 = 1.f; m._12 = 0.f; m._13 = 0.f; m._14 = 0.f;
            m._21 = 0.f; m._22 = 1.f; m._23 = 0.f; m._24 = 0.f;
            m._31 = 0.f; m._32 = 0.f; m._33 = 1.f; m._34 = 0.f;
            m._41 = 0.f; m._42 = 0.f; m._43 = 0.f; m._44 = 1.f;
            return m;
        }

        //! make 3D identity matrix
        inline matrix3x3& make_identity(matrix3x3& m)
        {
            m._11 = 1.f; m._12 = 0.f; m._13 = 0.f;
            m._21 = 0.f; m._22 = 1.f; m._23 = 0.f;
            m._31 = 0.f; m._32 = 0.f; m._33 = 1.f;
            return m;
        }

        //! make a 3D row major rotation matrix
        inline matrix3x3& make_rotation(matrix3x3& m, float _theta)
        {
            m._11 = cosf(_theta); m._12 = sinf(_theta); m._13 = 0.f;
            m._21 = -sinf(_theta); m._22 = cosf(_theta); m._23 = 0.f;
            m._31 = 0.f; m._32 = 0.f; m._33 = 1.f;
            return m;
        }

        //! make 4D row major rotation matrix against Z axis
        inline matrix4x4& make_rotation_z(matrix4x4& m, float _theta)
        {
            m._11 = cosf(_theta); m._12 = -sinf(_theta); m._13 = 0.f; m._14 = 0.f;
            m._21 = sinf(_theta); m._22 = cosf(_theta); m._23 = 0.f; m._24 = 0.f;
            m._31 = 0.f; m._32 = 0.f; m._33 = 1.f; m._34 = 0.f;
            m._41 = 0.f; m._42 = 0.f; m._43 = 0.f; m._44 = 1.f;
            return m;
        }

        //! make 4D row major rotation matrix against Y axis
        inline matrix4x4& make_rotation_y(matrix4x4& m, float _theta)
        {
            m._11 = cosf(_theta); m._12 = 0.f; m._13 = -sinf(_theta); m._14 = 0.f;
            m._21 = 0.f; m._22 = 1.f; m._23 = 0.f; m._24 = 0.f;
            m._31 = sinf(_theta); m._32 = 0.f; m._33 = cosf(_theta); m._34 = 0.f;
            m._41 = 0.f; m._42 = 0.f; m._43 = 0.f; m._44 = 1.f;
            return m;
        }

        //! make 4D row major rotation matrix against X axis
        inline matrix4x4& make_rotation_x(matrix4x4& m, float _theta)
        {
            m._11 = 1.f; m._12 = 0.f; m._13 = 0.f; m._14 = 0.f;
            m._21 = 0.f; m._22 = cosf(_theta); m._23 = sinf(_theta); m._24 = 0.f;
            m._31 = 0.f; m._32 = -sinf(_theta); m._33 = cosf(_theta); m._34 = 0.f;
            m._41 = 0.f; m._42 = 0.f; m._43 = 0.f; m._44 = 1.f;
            return m;
        }

        //! make 4D row major complete rotation matrix
        inline matrix4x4& make_rotation(matrix4x4& m, const vector3d& v)
        {
            matrix4x4 rotX, rotY, rotZ;
            make_rotation_x(rotX, v.x);
            make_rotation_y(rotY, v.y);
            make_rotation_z(rotZ, v.z);
            m = matrix_multiply(rotX, matrix_multiply(rotY, rotZ));
            return m;
        }

        //! make 3D row major translation matrix
        inline matrix3x3& make_translation(matrix3x3& m, const vector2d& v)
        {
            m._11 = 1.f; m._12 = 0.f; m._13 = v.x;
            m._21 = 0.f; m._22 = 1.f; m._23 = v.y;
            m._31 = 0.f; m._32 = 0.f; m._33 = 1.f;
            return m;
        }

        //! make 4D row major translation matrix
        inline matrix4x4& make_translation(matrix4x4& m, const vector3d& v)
        {
            m._11 = 1.f; m._12 = 0.f; m._13 = 0.f; m._14 = v.x;
            m._21 = 0.f; m._22 = 1.f; m._23 = 0.f; m._24 = v.y;
            m._31 = 0.f; m._32 = 0.f; m._33 = 1.f; m._34 = v.z;
            m._41 = 0.f; m._42 = 0.f; m._43 = 0.f; m._44 = 1.f;
            return m;
        }

        //! make 3D scale matrix
        inline matrix3x3& make_scale(matrix3x3& m, const vector2d& v)
        {
            m._11 = v.x; m._12 = 0.f; m._13 = 0.f;
            m._21 = 0.f; m._22 = v.y; m._23 = 0.f;
            m._31 = 0.f; m._32 = 0.f; m._33 = 1.f;
            return m;
        }

        //! make 4D scale translation matrix
        inline matrix4x4& make_scale(matrix4x4& m, const vector3d& v)
        {
            m._11 = v.x; m._12 = 0.f; m._13 = 0.f; m._14 = 0.f;
            m._21 = 0.f; m._22 = v.y; m._23 = 0.f; m._24 = 0.f;
            m._31 = 0.f; m._32 = 0.f; m._33 = v.z; m._34 = 0.f;
            m._41 = 0.f; m._42 = 0.f; m._43 = 0.f; m._44 = 1.f;
            return m;
        }

        inline matrix4x4& make_transpose(matrix4x4& m)
        {
            swap2_begin(float);
            swap2(m._12, m._21);
            swap2(m._13, m._31);
            swap2(m._14, m._41);
            swap2(m._23, m._32);
            swap2(m._24, m._42);
            swap2(m._34, m._43);
            swap2_end();
            return m;
        }

        inline vector4d cross4d(const vector4d& v1, const vector4d& v2, const vector4d& v3)
        {
            vector4d out;
            out.x = v1.y * (v2.z * v3.w - v3.z * v2.w) - v1.z * (v2.y * v3.w - v3.y * v2.w) + v1.w * (v2.y * v3.z - v2.z *v3.y);
            out.y = -(v1.x * (v2.z * v3.w - v3.z * v2.w) - v1.z * (v2.x * v3.w - v3.x * v2.w) + v1.w * (v2.x * v3.z - v3.x * v2.z));
            out.z = v1.x * (v2.y * v3.w - v3.y * v2.w) - v1.y * (v2.x *v3.w - v3.x * v2.w) + v1.w * (v2.x * v3.y - v3.x * v2.y);
            out.w = -(v1.x * (v2.y * v3.z - v3.y * v2.z) - v1.y * (v2.x * v3.z - v3.x *v2.z) + v1.z * (v2.x * v3.y - v3.x * v2.y));
            return out;
        }

        inline float matrix_determinant(const matrix4x4 m)
        {
            vector4d minor, v1, v2, v3;
            v1.x = m.m[0][0]; v1.y = m.m[1][0]; v1.z = m.m[2][0]; v1.w = m.m[3][0];
            v2.x = m.m[0][1]; v2.y = m.m[1][1]; v2.z = m.m[2][1]; v2.w = m.m[3][1];
            v3.x = m.m[0][2]; v3.y = m.m[1][2]; v3.z = m.m[2][2]; v3.w = m.m[3][2];
            minor = cross4d(v1, v2, v3);
            return - (m.m[0][3] * minor.x + m.m[1][3] * minor.y + m.m[2][3] * minor.z + m.m[3][3] * minor.w);
        }

        matrix4x4 make_inverse(const matrix4x4 &m);

        void transform_coord_array(std::vector<vector3d>& vertices, const matrix4x4& m);

        void RGBtoHSV( float r, float g, float b, float *h, float *s, float *v );
        void HSVtoRGB( float h, float s, float v, float *r, float *g, float *b );

        float HSV_distance_from_RGB(const vector3d & c1, const vector3d & c2);
    }
}

#endif
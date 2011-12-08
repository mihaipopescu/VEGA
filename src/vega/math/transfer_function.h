#ifndef __VEGA_MATH_TRANSFER_FUNCTION_H__
#define __VEGA_MATH_TRANSFER_FUNCTION_H__

#include <vector>
#include "../common/types.h"
#include "vector4d.h"


namespace vega
{
    namespace math
    {
        //! Spline control point for transfer function.
        /*!
         * Transfer control point is used to define a control point on the spline for the transfer function. \n
         * It's composed of a RGBA color stored as a normalized 4D vector and an associated Iso value.
         */
		struct transfer_control_point
        {
            vector4d Color;         //!< the color of the control point
            unsigned char IsoValue; //!< the associated iso value of the control point

            //! default c-tor
            transfer_control_point() : Color(vector4d(0.f, 0.f, 0.f, 0.f)), IsoValue(0) { }
            //! c-tor used for RGB color knots
            transfer_control_point(float r, float g, float b, unsigned char iso) : Color(vector4d(r,g,b,1.f)), IsoValue(iso) { }
            //! complete c-tor
            transfer_control_point(float r, float g, float b, float a, unsigned char iso) : Color(vector4d(r,g,b,a)), IsoValue(iso) { }
            //! c-tor used for alpha knots
            transfer_control_point(float alpha, unsigned char iso) : Color(vector4d(0.f, 0.f, 0.f, alpha)), IsoValue(iso) { }
        };

        //! 4d cubic polynomial representation.
        /*!
         * Cubic spline is computed from a set of control points and preforms cubic interpolation. \n
         * Based on Tim Lambert's natural cubic spline: http://www.cse.unsw.edu.au/~lambert/splines/source.html \n
         * The 4d cubic is defined by: $f(x) = a + b*x + c*x^2 +d*x^3$, where a, b, c, d are 4d vectors.
         */
        class cubic4d
        {
        public:
            //! c-tor for cubic4d
            cubic4d(const vector4d &_a, const vector4d &_b, const vector4d &_c, const vector4d &_d) : a(_a), b(_b), c(_c), d(_d) { }

            //! Evaluates this 4d cubic polynomial at point s.
            /*!
             * The 4d cubic polynomial is evaluated at point s using the formula: $f(s) = a + b*s + c*s^2 +d*s^3$;
             * \param s point to evaluate.
             * \return The 4d cubic polynomial value at point s.
             */
            vector4d evaluate(float s) const { return (((d * s) + c) * s + b) * s + a; }

        private:
            vector4d a, b, c, d; //!< polynomial coeficients
        };

        //! Creates a transfer function.
        /*!
         * Creates a palette from a 4d spline defined by control points or from an actual palette. \n
         * Because the algorithm is for the same in all dimensions, the algorithm is extended to the 4D space to compute the transfer function for the RGB color channels plus the alpha channel.\n
         * Based on <a href="http://graphicsrunner.blogspot.com/2009/01/volume-rendering-102-transfer-functions.html">Kyle Hayward's Volume Rendering Tutorial</a>.
         * \sa cubic4d, transfer_control_point
         */
        class transfer_function
        {
        public:
            //! Constructs a transfer function taking a vector of palette knots.
            transfer_function(const std::vector<transfer_control_point>& vPaletteKnots);
            //! Constructs a transfer function after computing a spline from the color and alpha knots.
            transfer_function(const std::vector<transfer_control_point>& vColorKnots, const std::vector<transfer_control_point>& vAlphaKnots);

        public:
            r8g8b8a8 myTransferArray[256]; //!< The generated transfer function array.

        private:
            //! Computes a cubic 4d spline.
            /*!
             * \param v the vector of spline control points.
             * \param cubic4dspline the computed 4d spline.
             */
            void compute_cubic_4d_spline(const std::vector<transfer_control_point> &vControlPoints, std::vector<cubic4d>& cubic4dspline) const;
        };
    }
}

#endif // __VEGA_MATH_TRANSFER_FUNCTION_H__
#ifndef __VEGA_ALGORITHM_RESAMPLE_FILTERS_H__
#define __VEGA_ALGORITHM_RESAMPLE_FILTERS_H__

#include "../../math/common.h"


namespace vega
{
    namespace algorithm
    {
        namespace resample
        {
            class generic_filter
            {
            public:
                generic_filter (double dWidth) : m_dWidth (dWidth) {}

                double GetWidth() const 			{ return m_dWidth; }
                void   SetWidth (double dWidth)     { m_dWidth = dWidth; }

                virtual double Filter (double dVal) const = 0;
            protected:
                double  m_dWidth;
            };

            class box_filter : public generic_filter
            {
            public:
                box_filter() : generic_filter(0.5) {}

                double Filter (double dVal) const { return (fabs(dVal) <= m_dWidth ? 1.0 : 0.0); }
            };

            class bilinear_filter : public generic_filter
            {
            public:
                bilinear_filter () : generic_filter(1) {}

                double Filter (double dVal) const {
                    dVal = fabs(dVal);
                    return (dVal < m_dWidth ? m_dWidth - dVal : 0.0);
                }
            };

            class bicubic_filter : public generic_filter
            {
            public:
                bicubic_filter (double b = (1/(double)3), double c = (1/(double)3)) : generic_filter(2) {
                    p0 = (6 - 2*b) / 6;
                    p2 = (-18 + 12*b + 6*c) / 6;
                    p3 = (12 - 9*b - 6*c) / 6;
                    q0 = (8*b + 24*c) / 6;
                    q1 = (-12*b - 48*c) / 6;
                    q2 = (6*b + 30*c) / 6;
                    q3 = (-b - 6*c) / 6;
                }

                double Filter(double dVal) const {
                    dVal = fabs(dVal);
                    if(dVal < 1)
                        return (p0 + dVal*dVal*(p2 + dVal*p3));
                    if(dVal < 2)
                        return (q0 + dVal*(q1 + dVal*(q2 + dVal*q3)));
                    return 0;
                }
            protected:
                double p0, p2, p3;
                double q0, q1, q2, q3;
            };

            class lanczos3_filter : public generic_filter
            {
            public:
                lanczos3_filter() : generic_filter(3) {}
                virtual ~lanczos3_filter() {}

                double Filter(double dVal) const {
                    dVal = fabs(dVal);
                    if(dVal < m_dWidth)     {
                        return (sinc(dVal) * sinc(dVal / m_dWidth));
                    }
                    return 0;
                }

            private:
                inline double sinc(double value) const {
                    if(value != 0) {
                        value *= math::PI;
                        return (sin(value) / value);
                    }
                    return 1;
                }
            };

            class bspline_filter : public generic_filter
            {
            public:
                bspline_filter() : generic_filter(2) {}

                double Filter(double dVal) const {

                    dVal = fabs(dVal);
                    if(dVal < 1) return (4 + dVal*dVal*(-6 + 3*dVal)) / 6;
                    if(dVal < 2) {
                        double t = 2 - dVal;
                        return (t*t*t / 6);
                    }
                    return 0;
                }
            };
        }
    }
}


#endif // __VEGA_ALGORITHM_RESAMPLE_FILTERS_H__
/*! 
 *  \brief     weights_talbe class.
 *  \details   This class is used for resampling.
 *  \author    Mihai Popescu
 *  \date      01/03/2009
 *  \copyright GNU Public License.
 */
#ifndef __VEGA_ALGORITHM_RESAMPLE_WEIGHTS_TABLE_H__
#define __VEGA_ALGORITHM_RESAMPLE_WEIGHTS_TABLE_H__

#include "../../common/types.h"
#include "Filters.h"

namespace vega
{
    namespace algorithm
    {
        namespace resample
        {
            class weights_table
            {
            public:

                weights_table(const generic_filter *pFilter, uint32 uDstSize, uint32 uSrcSize);
                ~weights_table();

                // Retrieve a filter weight, given source and destination positions
                double getWeight(int dst_pos, int src_pos) const {
                    return m_WeightTable[dst_pos].Weights[src_pos];
                }

                // Retrieve left boundary of source line buffer
                int getLeftBoundary(int dst_pos) const {
                    return m_WeightTable[dst_pos].Left;
                }

                // Retrieve right boundary of source line buffer
                int getRightBoundary(int dst_pos) const {
                    return m_WeightTable[dst_pos].Right;
                }

            private:
                typedef struct
                {
                    double *Weights;			//!< Normalized weights of neighboring pixels
                    int Left, Right;			//!< Bounds of source pixels window
                } sContribution;

                sContribution * m_WeightTable;  //!< Row (or column) of contribution weights
                uint32 m_WindowSize;            //!< Filter window size (of affecting source pixels)
                uint32 m_LineLength;            //!< Length of line (no. of rows / cols)
            };

        }
    }
}


#endif // __VEGA_ALGORITHM_RESAMPLE_WEIGHTS_TABLE_H__
#include "resizeable_image.h"

using namespace vega::algorithm::resample;


void vega::data::resizeable_image::horizontal_filter( const algorithm::resample::generic_filter * filter, uint32 dst_width, uint32 dst_height )
{
    if (dst_width == myWidth)
    {
        // No scaling required, just copy
        myResizedImage = myRGBData;
        return;
    }

    weights_table wt(filter, dst_width, myWidth);

    for (uint32 u = 0; u < dst_height; u++)
    {
        // scale each row
        uint8 *pDstRow = &(myResizedImage[3 * u * dst_width]);
        uint8 *pSrcRow = &(myRGBData[3 * u * myWidth]);

        for (uint32 x = 0; x < dst_width; x++) 
        {
            // Loop through row
            uint8 r = 0;
            uint8 g = 0;
            uint8 b = 0;
            int iLeft = wt.getLeftBoundary(x);	// Retrieve left boundaries
            int iRight = wt.getRightBoundary(x);  // Retrieve right boundaries
            for (int i = iLeft; i <= iRight; i++)
            {
                // Scan between boundaries
                // Accumulate weighted effect of each neighboring pixel
                r += (uint8)((wt.getWeight(x, i-iLeft) * (double)(pSrcRow[i*3]))); 
                g += (uint8)((wt.getWeight(x, i-iLeft) * (double)(pSrcRow[i*3 + 1]))); 
                b += (uint8)((wt.getWeight(x, i-iLeft) * (double)(pSrcRow[i*3 + 2]))); 
            } 

            // set destination row
            pDstRow[3*x] = r;
            pDstRow[3*x + 1] = g;
            pDstRow[3*x + 2] = b;
        }
    }
}

void vega::data::resizeable_image::vertical_filter( const algorithm::resample::generic_filter * filter, uint32 dst_width, uint32 dst_height )
{
    if (myHeight == dst_height)
    {
        myResizedImage = myRGBData;
        return;
    }

    weights_table wt(filter, dst_height, myHeight);

    for (uint32 u = 0; u < dst_width; u++)
    {
        // Step through columns
        for (uint32 y = 0; y < dst_height; y++) 
        {
            // Loop through column
            uint8 r = 0;
            uint8 g = 0;
            uint8 b = 0;
            int iLeft = wt.getLeftBoundary(y);	// Retrieve left boundaries
            int iRight = wt.getRightBoundary(y);  // Retrieve right boundaries
            for (int i = iLeft; i <= iRight; i++)
            {
                // Scan between boundaries
                // Accumulate weighted effect of each neighboring pixel
                uint8 *src = &myRGBData[3*(i * myWidth + u)];
                r += (uint8)((wt.getWeight(y, i-iLeft) * (double)(src[0])));
                g += (uint8)((wt.getWeight(y, i-iLeft) * (double)(src[1])));
                b += (uint8)((wt.getWeight(y, i-iLeft) * (double)(src[2])));
            }

            uint8 *dst = &myResizedImage[3*(y * dst_width + u)];
            dst[0] = r;
            dst[1] = g;
            dst[2] = b;
        }
    }

}

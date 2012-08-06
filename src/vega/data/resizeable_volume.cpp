#include "resizeable_volume.h"

using namespace vega::algorithm::resample;


void vega::data::resizeable_volume::horizontal_filter( const algorithm::resample::generic_filter * filter, uint32 dst_width, uint32 dst_height, uint32 dst_depth )
{
	if (dst_width == myWidth)
    {
        // No scaling required, just copy
		myResizedVolume = myVoxelArray;
        return;
    }

    weights_table wt(filter, dst_width, myWidth);

	for (uint32 z = 0; z < dst_depth; z++)
	{
		for (uint32 y = 0; y < dst_height; y++)
		{
			uint8 *pDstRow = &(myResizedVolume[y * dst_width + z * dst_width * dst_height]);
			uint8 *pSrcRow = &(myVoxelArray[y * myWidth + z * myWidth * myHeight]);

			for (uint32 x = 0; x < dst_width; x++) 
			{
				uint8 v = 0;
				int iLeft = wt.getLeftBoundary(x);	// Retrieve left boundaries
				int iRight = wt.getRightBoundary(x);  // Retrieve right boundaries
				for (int i = iLeft; i <= iRight; i++)
				{
					// Scan between boundaries
					// Accumulate weighted effect of each neighboring pixel
					v += (uint8)((wt.getWeight(x, i-iLeft) * (double)(pSrcRow[i]))); 
				} 

				// set destination row
				pDstRow[x] = v;
			}
		}
	}
}

void vega::data::resizeable_volume::vertical_filter( const algorithm::resample::generic_filter * filter, uint32 dst_width, uint32 dst_height, uint32 dst_depth )
{
	if (myHeight == dst_height)
    {
		// No scaling required, just copy
		myResizedVolume = myVoxelArray;
        return;
    }

    weights_table wt(filter, dst_height, myHeight);

	for (uint32 z = 0; z < dst_depth; z++)
    {
		for (uint32 x = 0; x < dst_width; x++)
		{
			for (uint32 y = 0; y < dst_height; y++) 
			{
				uint8 v = 0;
				int iLeft = wt.getLeftBoundary(y);	// Retrieve left boundaries
				int iRight = wt.getRightBoundary(y);  // Retrieve right boundaries
				for (int i = iLeft; i <= iRight; i++)
				{
					// Scan between boundaries
					// Accumulate weighted effect of each neighboring pixel
					v += (uint8)((wt.getWeight(y, i-iLeft) * (double)(myVoxelArray[z * myWidth * myHeight + i * myWidth + x])));
				}

				myResizedVolume[z * dst_height * dst_width + y * dst_width + x] = v;
			}
		}
	}
}

void vega::data::resizeable_volume::depth_filter( const algorithm::resample::generic_filter * filter, uint32 dst_width, uint32 dst_height, uint32 dst_depth )
{
    if (myDepth == dst_depth)
    {
		// No scaling required, just copy
		myResizedVolume = myVoxelArray;
        return;
    }

    weights_table wt(filter, dst_depth, myDepth);

	for (uint32 x = 0; x < dst_width; x++)
	{
		for (uint32 y = 0; y < dst_height; y++) 
		{
			for (uint32 z = 0; z < dst_depth; z++)
		    {
				uint8 v = 0;
				int iLeft = wt.getLeftBoundary(z);	// Retrieve left boundaries
				int iRight = wt.getRightBoundary(z);  // Retrieve right boundaries
				for (int i = iLeft; i <= iRight; i++)
				{
					// Scan between boundaries
					// Accumulate weighted effect of each neighboring pixel
					v += (uint8)((wt.getWeight(z, i-iLeft) * (double)(myVoxelArray[i * myWidth * myHeight + y * myWidth + x])));
				}

				myResizedVolume[z * dst_height * dst_width + y * dst_width + x] = v;
			}
		}
	}
}

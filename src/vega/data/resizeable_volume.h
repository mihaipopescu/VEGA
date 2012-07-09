#ifndef __VEGA_DATA_RESIZEABLE_VOLUME_H__
#define __VEGA_DATA_RESIZEABLE_VOLUME_H__

#include <memory>
#include "volume.h"
#include "../algorithm/resample/filters.h"
#include "../algorithm/resample/weights_table.h"


namespace vega
{
    namespace data
    {
        class resizeable_volume : public volume
        {
        public:

            template <class T>
            void resample(uint32 dst_width, uint32 dst_height, uint32 dst_depth)
            {
                T filter;

                void (resizeable_volume::*ff[3])(const algorithm::resample::generic_filter * filter, uint32 dst_width, uint32 dst_height, uint32 dst_depth);
                uint32 filter_size[9] = 
                {
                    myWidth, myHeight, myDepth,
                    myWidth, myHeight, myDepth,
                    dst_width, dst_width, dst_depth
                };

                if( dst_width * myHeight * myDepth < myWidth * dst_height * myDepth && 
                    dst_width * myHeight * myDepth < myWidth * myHeight * dst_depth )
                {
                    ff[0] = &resizeable_volume::horizontal_filter;
                    filter_size[0] = dst_width;
                    filter_size[3] = dst_width;

                    if( dst_width * dst_height * myDepth < dst_width * myHeight * dst_depth )
                    {
                        filter_size[1] = dst_height;
                        filter_size[5] = dst_depth;
                        
                        ff[1] = &resizeable_volume::vertical_filter;
                        ff[2] = &resizeable_volume::depth_filter;
                    }
                    else
                    {
                        filter_size[2] = dst_depth;
                        filter_size[4] = dst_height;
                        
                        ff[1] = &resizeable_volume::depth_filter;
                        ff[2] = &resizeable_volume::vertical_filter;
                    }
                } 
                else
                {
                    if( myWidth * dst_height * myDepth < dst_width * myHeight * myDepth && 
                        myWidth * dst_height * myDepth < myWidth * myHeight * dst_depth )
                    {
                        ff[0] = &resizeable_volume::vertical_filter;
                        filter_size[1] = dst_height;
                        filter_size[4] = dst_height;

                        if( dst_width * dst_height * myDepth < myWidth * dst_height * dst_depth )
                        {
                            filter_size[0] = dst_width;
                            filter_size[5] = dst_depth;

                            ff[1] = &resizeable_volume::horizontal_filter;
                            ff[2] = &resizeable_volume::depth_filter;
                        }
                        else
                        {
                            filter_size[2] = dst_depth;
                            filter_size[3] = dst_width;

                            ff[1] = &resizeable_volume::depth_filter;
                            ff[2] = &resizeable_volume::horizontal_filter;
                        }
                    }
                    else
                    {
                        if( myWidth * myHeight * dst_depth < dst_width * myHeight * myDepth && 
                            myWidth * myHeight * dst_depth < myWidth * dst_height * myDepth )
                        {
                            ff[0] = &resizeable_volume::depth_filter;
                            filter_size[2] = dst_depth;
                            filter_size[5] = dst_depth;

                            if( dst_width * myHeight * dst_depth < myWidth * dst_height * dst_depth )
                            {
                                filter_size[0] = dst_width;
                                filter_size[4] = dst_height;

                                ff[1] = &resizeable_volume::horizontal_filter;
                                ff[2] = &resizeable_volume::vertical_filter;
                            }
                            else
                            {
                                filter_size[1] = dst_height;
                                filter_size[3] = dst_width;

                                ff[1] = &resizeable_volume::horizontal_filter;
                                ff[2] = &resizeable_volume::vertical_filter;
                            }
                        }
                    }
                }
                

                for(int i=0; i<3; ++i)
                {
                    myResizedVolume.resize(filter_size[i*3] * filter_size[i*3+1] * filter_size[i*3+2], 0);
                    (this->*ff[i])(&filter, filter_size[i*3], filter_size[i*3+1], filter_size[i*3+2]);
                    myWidth = filter_size[i*3];
                    myHeight = filter_size[i*3+1];
                    myDepth = filter_size[i*3+2];
                    myVoxelArray = myResizedVolume;
                }                
            }

        private:
            //! Performs horizontal image filtering
            void horizontal_filter(const algorithm::resample::generic_filter * filter, uint32 dst_width, uint32 dst_height, uint32 dst_depth);

            //! Performs vertical image filtering
            void vertical_filter(const algorithm::resample::generic_filter * filter, uint32 dst_width, uint32 dst_height, uint32 dst_depth);

            //! Performs depth image filtering
            void depth_filter(const algorithm::resample::generic_filter * filter, uint32 dst_width, uint32 dst_height, uint32 dst_depth);


            std::vector<voxel> myResizedVolume;
        };
    }
}


#endif // __VEGA_DATA_RESIZEABLE_VOLUME_H__
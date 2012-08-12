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
            resizeable_volume(const std::string& _FileName) : volume(_FileName, false) { }

            template <class T>
            void resample(uint32 dst_width, uint32 dst_height, uint32 dst_depth)
            {
                T filter;

                void (resizeable_volume::*ff[3])(const algorithm::resample::generic_filter * filter, uint32 dst_width, uint32 dst_height, uint32 dst_depth);

                struct filter_pass
                {
                    uint32 width;
                    uint32 height;
                    uint32 depth;
                } 
                filter_size[3] = 
                {   {myWidth, myHeight, myDepth},
                    {myWidth, myHeight, myDepth},
                    {dst_width, dst_height, dst_depth}
                };

                if( dst_width * myHeight * myDepth <= myWidth * dst_height * myDepth && 
                    dst_width * myHeight * myDepth <= myWidth * myHeight * dst_depth )
                {
                    ff[0] = &resizeable_volume::horizontal_filter;
                    filter_size[0].width = filter_size[1].width = dst_width;

                    if( dst_width * dst_height * myDepth <= dst_width * myHeight * dst_depth )
                    {
                        filter_size[1].height = dst_height;
                        
                        ff[1] = &resizeable_volume::vertical_filter;
                        ff[2] = &resizeable_volume::depth_filter;
                    }
                    else
                    {
                        filter_size[1].depth = dst_depth;
                        
                        ff[1] = &resizeable_volume::depth_filter;
                        ff[2] = &resizeable_volume::vertical_filter;
                    }
                } 
                else
                {
                    if( myWidth * dst_height * myDepth <= dst_width * myHeight * myDepth && 
                        myWidth * dst_height * myDepth <= myWidth * myHeight * dst_depth )
                    {
                        ff[0] = &resizeable_volume::vertical_filter;
                        filter_size[0].height = filter_size[1].height = dst_height;

                        if( dst_width * dst_height * myDepth <= myWidth * dst_height * dst_depth )
                        {
                            filter_size[1].width = dst_width;

                            ff[1] = &resizeable_volume::horizontal_filter;
                            ff[2] = &resizeable_volume::depth_filter;
                        }
                        else
                        {
                            filter_size[1].depth = dst_depth;

                            ff[1] = &resizeable_volume::depth_filter;
                            ff[2] = &resizeable_volume::horizontal_filter;
                        }
                    }
                    else
                    {
                        //if( myWidth * myHeight * dst_depth < dst_width * myHeight * myDepth && 
                            //myWidth * myHeight * dst_depth < myWidth * dst_height * myDepth )
                        {
                            ff[0] = &resizeable_volume::depth_filter;
                            filter_size[0].depth = filter_size[1].depth = dst_depth;

                            if( dst_width * myHeight * dst_depth <= myWidth * dst_height * dst_depth )
                            {
                                filter_size[1].width = dst_width;

                                ff[1] = &resizeable_volume::horizontal_filter;
                                ff[2] = &resizeable_volume::vertical_filter;
                            }
                            else
                            {
                                filter_size[1].height = dst_height;

                                ff[1] = &resizeable_volume::horizontal_filter;
                                ff[2] = &resizeable_volume::vertical_filter;
                            }
                        }
                    }
                }
                

                for(int i=0; i<3; ++i)
                {
                    myResizedVolume.resize(filter_size[i].width * filter_size[i].height * filter_size[i].depth, 0);
                    (this->*ff[i])(&filter, filter_size[i].width, filter_size[i].height, filter_size[i].depth);
                    myWidth = filter_size[i].width;
                    myHeight = filter_size[i].height;
                    myDepth = filter_size[i].depth;
                    myVoxelArray = myResizedVolume;
                }

                paint_voxels();
                myRawDataIsDirty = true;
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
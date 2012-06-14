#ifndef __VEGA_DATA_RESIZEABLE_IMAGE_H__
#define __VEGA_DATA_RESIZEABLE_IMAGE_H__

#include <memory>
#include "image.h"
#include "../algorithm/resample/filters.h"
#include "../algorithm/resample/weights_table.h"

namespace vega
{
    namespace data
    {
        class resizeable_image : public image
        {
        public:

            template <class T>
            void resample(uint32 dst_width, uint32 dst_height)
            {
                T filter;

                // decide which filtering order (xy or yx) is faster for this mapping
                if(dst_width * myHeight <= dst_height * myWidth) 
                {
                    myResizedImage.resize(3 * dst_width * myHeight, 0);

                    horizontal_filter(&filter, dst_width, myHeight);

                    myRGBData = myResizedImage;
                    myWidth = dst_width;
                    myResizedImage.resize(3 * dst_width * dst_height, 0);

                    vertical_filter(&filter, dst_width, dst_height);
                }
                else
                {
                    myResizedImage.resize(3 * myWidth * dst_height, 0);

                    vertical_filter(&filter, myWidth, dst_height);

                    myRGBData = myResizedImage;
                    myHeight = dst_height;
                    myResizedImage.resize(3 * dst_width * dst_height, 0);

                    horizontal_filter(&filter, dst_width, dst_height);
                }

                myRGBData = myResizedImage;
                myWidth = dst_width;
                myHeight = dst_height;
                myResizedImage.clear();
            }

        private:
            //! Performs horizontal image filtering
            void horizontal_filter(const algorithm::resample::generic_filter * filter, uint32 dst_width, uint32 dst_height);

            //! Performs vertical image filtering
            void vertical_filter(const algorithm::resample::generic_filter * filter, uint32 dst_width, uint32 dst_height);

            std::vector<uint8> myResizedImage;
        };
    }
}


#endif // __VEGA_DATA_RESIZEABLE_IMAGE_H__
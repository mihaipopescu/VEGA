#ifndef __VEGA_RENDER_IMAGE_FILE_H__
#define __VEGA_RENDER_IMAGE_FILE_H__

#include <vector>
#include "common.h"
#include "../math/vector3d.h"
#include "../common/types.h"

namespace vega
{
    namespace data
    {
        class image : public i_model
        {
        public:
            image();

            virtual bool create(const char *szFileName);
            virtual void create(const image & img) { *this = img; }

            size_t get_width() const { return myWidth; }
            size_t get_height() const { return myHeight; }
            
            math::vector3d get_pixel(size_t x, size_t y) const
            {
                size_t index = 3*(y*myWidth + x);
                if( index >= myRGBData.size() )
                    return math::vector3d(math::INFINIT, math::INFINIT, math::INFINIT);

                return math::vector3d(myRGBData[index+0]/255.f, myRGBData[index+1]/255.f, myRGBData[index+2]/255.f);
            }

            bool set_pixel_contrast(size_t x, size_t y, float fContrast)
            {
                size_t index = 3*(y*myWidth + x);
                if( index >= myRGBData.size() )
                    return false;
                
                float r = myRGBData[index+0] * fContrast;
                float g = myRGBData[index+1] * fContrast;
                float b = myRGBData[index+2] * fContrast;

                myRGBData[index+0] = r > 255.f ? 255 : (uint8)r;
                myRGBData[index+1] = g > 255.f ? 255 : (uint8)g;
                myRGBData[index+2] = b > 255.f ? 255 : (uint8)b;

                return true;
            }

            const void * get_raw_data() const { return static_cast<const void*>(myRGBData.data()); }

        protected:
            std::vector<uint8> myRGBData;
            size_t myWidth;
            size_t myHeight;
        };
    }
}

#endif // __VEGA_RENDER_IMAGE_FILE_H__
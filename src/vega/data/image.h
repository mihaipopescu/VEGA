#ifndef __VEGA_RENDER_IMAGE_FILE_H__
#define __VEGA_RENDER_IMAGE_FILE_H__

#include <vector>
#include "common.h"
#include "../math/vector3d.h"
#include "../common/types.h"
#include "../common/model_view_controller.h"

namespace vega
{
    namespace data
    {
        class image : public mvc::i_model
        {
        public:
            image(const std::string& _FileName);

            virtual bool create();

            uint16 get_width() const { return myWidth; }
            uint16 get_height() const { return myHeight; }
            
            bool get_pixel(uint16 x, uint16 y, math::vector3d& pixel) const;
            bool set_pixel_contrast(uint16 x, uint16 y, float fContrast);

            const void * get_raw_data() const { return static_cast<const void*>(myRGBData.data()); }

        protected:
            std::string myBMPFileName;
            std::vector<uint8> myRGBData;
            uint16 myWidth;
            uint16 myHeight;
        };
    }
}

#endif // __VEGA_RENDER_IMAGE_FILE_H__
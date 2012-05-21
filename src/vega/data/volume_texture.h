#ifndef __VEGA_DATA_VOLUME_TEXTURE_H__
#define __VEGA_DATA_VOLUME_TEXTURE_H__

#include <vector>
#include "../common/types.h"
#include "volume.h"
#include "../math/vector3d.h"
#include "../math/transformations.h"


namespace vega
{
    namespace data
    {
        class volume_texture
        {
        public:
            volume_texture();

            void create(const volume& v);

            uint16 get_width        () const { return myWidth; }
            uint16 get_height       () const { return myHeight; }
            uint16 get_depth        () const { return myDepth; }

            r8g8b8a8 get_voxel(uint16 x, uint16 y, uint16 z) const { return m_vPaintedVoxels.at(_I(x,y,z)); }
            void set_voxel(uint16 x, uint16 y, uint16 z, r8g8b8a8 voxel) { m_vPaintedVoxels[_I(x,y,z)] = voxel; }


            float get_voxel_color(uint16 x, uint16 y, uint16 z) const 
            {
                r8g8b8a8 o = get_voxel(x, y, z);

                float h, s, v;
                math::RGBtoHSV(o.R, o.G, o.B, &h, &s, &v);
                return h;
            }

            float  get_voxel_distance   (uint16 x1, uint16 y1, uint16 z1, uint16 x2, uint16 y2, uint16 z2) const
            {
                r8g8b8a8 v1 = get_voxel(x1, y1, z1);
                r8g8b8a8 v2 = get_voxel(x2, y2, z2);
                math::vector3d p1(v1.R, v1.G, v1.B);
                math::vector3d p2(v2.R, v2.G, v2.B);
                return math::HSV_distance_from_RGB(p1, p2); 
            }

        private:
            std::vector<r8g8b8a8>   m_vPaintedVoxels;
            uint16                  myWidth;
            uint16                  myHeight;
            uint16                  myDepth;
        };
    }
}



#endif // __VEGA_DATA_VOLUME_TEXTURE_H__
#include "volume_texture.h"


void vega::data::volume_texture::create( const volume &v )
{
    myWidth = v.get_width();
    myHeight = v.get_height();
    myDepth = v.get_depth();

    v.paint_voxels(m_vPaintedVoxels);
}


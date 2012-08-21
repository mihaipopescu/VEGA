#include "hexagonal_lattice.h"
#include "../common/logger.h"
#include "../math/transformations.h"
#include "../common/logger.h"

#include <iostream>
#include <vector>
#include <assert.h>


vega::data::hexagonal_lattice::hexagonal_lattice( const data::image & img )
{
    VEGA_LOG_FN;

    // Lattice L = {Lx, Ly};
    static const int Lx[2] = {2, -1};
    static const int Ly[2] = {0, 2};

    myWidth = img.get_width() / 2;
    myHeight = img.get_height() / 2;

    assert(myWidth > 0 && myHeight > 0);

    myLatticeCells.reserve(myWidth * myHeight);

    for(uint16 i=0;i<myHeight;++i)
    {
        for(uint16 j=0;j<myWidth;++j)
        {
            hexagon_node cell;
            cell.x = j;
            cell.y = i + j/2 + j%2;

            static const int a[6][2] = { {1, 1}, {0, 1}, {-1, 0}, {-1, -1}, {0, -1}, {1, 0} };

            for(int h=0; h<6; ++h)
            {
                int hj = cell.x + a[h][0];
                int hi = cell.y + a[h][1] - hj/2 - hj%2;
                
                if( hi >= 0 && hi < myHeight && hj >= 0 && hj < myWidth )
                    cell.hex[h] = hi *myWidth + hj;
                else
                    cell.hex[h] = -1;
            }

            // the hexagon pixel offsets from the upper left corner
            // 
            // x 1-2 o
            //  /   \
            // 3-4-5-6
            //  \   /
            // o 7-8 o
            //

            static const int hex_pixel_offset[8][2] = { {1, 0}, {2, 0}, {0, 1}, {1, 1}, {2, 1}, {3, 1}, {1, 2}, {2, 2}};

            math::vector3d& rgb = cell.color;

            int x = cell.x * Lx[0] + cell.y * Ly[0];
            int y = cell.x * Lx[1] + cell.y * Ly[1];

            // compute average color
            float k = 0;
            for(size_t h=0;h<8;++h)
            {
                math::vector3d pixel;
                
                if( img.get_pixel(x + hex_pixel_offset[h][0], y + hex_pixel_offset[h][1], pixel) )
                {
                    rgb = (rgb * k + pixel) * (1/(++k));
                }
            }

            myLatticeCells.push_back(cell);
        }
    }
}

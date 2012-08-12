#include "hexagonal_prismatic_lattice.h"
#include "../common/logger.h"

#include <assert.h>
#include <iostream>
#include <algorithm>


using namespace vega;
using namespace vega::math;


vega::data::hexagonal_prismatic_lattice::hexagonal_prismatic_lattice( const volume& v )
{
    SMART_LOG_FN;

    // Lattice L = {Lx, Ly, Lz};
    static const int Lx[3] = {2, -1, 0};
    static const int Ly[3] = {0, 2, 0};
    static const int Lz[3] = {0, 0, 1};

    myWidth = v.get_width() / 2;
    myHeight = v.get_height() / 2;
    myDepth = v.get_depth();

    assert(myWidth > 0 && myHeight > 0 && myDepth > 0);

    myLatticeCells.reserve(myWidth * myHeight * myDepth);

    for(uint16 k=0;k<myDepth;++k)
    {
        for(uint16 i=0;i<myHeight;++i)
        {
            for(uint16 j=0;j<myWidth;++j)
            {
                prismatic_hexagon_node cell;

                cell.x = j;
                cell.y = i + j/2 + j%2;
                cell.z = k;


                static const int hex_neighbor_offset[NEIGHBOR_COUNT][3] = 
                { 
                               {1, 1, 0}, {0, 1, 0}, {-1, 0, 0}, {-1, -1, 0}, {0, -1, 0}, {1, 0, 0},
                    {0, 0,-1}, 
#if NEIGHBOR_COUNT==20
                               {1, 1,-1}, {0, 1,-1}, {-1, 0,-1}, {-1, -1,-1}, {0, -1,-1}, {1, 0,-1},
#endif
                    {0, 0, 1}, 
#if NEIGHBOR_COUNT==20                               
                               {1, 1, 1}, {0, 1, 1}, {-1, 0, 1}, {-1, -1, 1}, {0, -1, 1}, {1, 0, 1}
#endif
                };

                for(int h=0; h<NEIGHBOR_COUNT; ++h)
                {
                    int hj = cell.x + hex_neighbor_offset[h][0];
                    int hi = cell.y + hex_neighbor_offset[h][1] - hj/2 - hj%2;
                    int hk = cell.z + hex_neighbor_offset[h][2];

                    if( hi >= 0 && hi < myHeight && hj >= 0 && hj < myWidth && hk >=0 && hk < myDepth )
                        cell.hex[h] = hk * myWidth * myHeight + hi * myWidth + hj;
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
                
                math::vector3d vertex = cell.get_vertex();

                cell.density = 0.f;

                // compute average density
                int k = 0;
                for(size_t h=0;h<8;++h)
                {
                    math::vector3d pixel;
                    int xx = (int)vertex.x + hex_pixel_offset[h][0];
                    int yy = (int)vertex.y + hex_pixel_offset[h][1];
                    int zz = (int)vertex.z;
                    if( xx >= 0 && yy >= 0 && zz >= 0 && xx < v.get_width() && yy < v.get_height() && zz < v.get_depth() )
                    {
                        float vx = v.get_voxel(xx, yy, zz);
                        cell.density = (cell.density * k + vx) * (1.f/(++k));
                    }
                }

                myLatticeCells.push_back(cell);
            }
        }

    }
}

math::vector3d vega::data::hexagonal_prismatic_lattice::prismatic_hexagon_node::get_vertex() const
{
    // Lattice L = {Lx, Ly, Lz};
    static const int Lx[3] = {2, -1, 0};
    static const int Ly[3] = {0, 2, 0};
    static const int Lz[3] = {0, 0, 1};

    int xx = x * Lx[0] + y * Ly[0] + z * Lz[0];
    int yy = x * Lx[1] + y * Ly[1] + z * Lz[1];
    int zz = x * Lx[2] + y * Ly[2] + z * Lz[2];

    return math::vector3d((float)xx, (float)yy, (float)zz);
}

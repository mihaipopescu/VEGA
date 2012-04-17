#include "compact_hexagonal_lattice.h"

using namespace vega;


void vega::data::compact_hexagonal_lattice::create( uint8 width, uint8 height, float span )
{
    for(uint8 y=0; y<height; ++y)
    {
        float dx = y % 2 == 0 ? 0 : span / 2;
        float fy = y * span;
        for(uint8 x=0; x<width; ++x)
        {
            uint16 idx = y * width + x;

            myLattice.push_back(math::vector2d(x * span + dx, fy));
            if( y % 2 == 0 )
            {
                if( x % 3 == 2 )
                {
                    myLines.push_back(idx);
                    myLines.push_back(idx-1);
                }
            }
            else
            {
                switch( x % 3 )
                {
                case 0:
                    if( x < width-1 && y > 0 )
                    {
                        myLines.push_back(idx);
                        myLines.push_back(idx-width + 1);
                    }

                    if( x > 0 )
                    {
                        myLines.push_back(idx);
                        myLines.push_back(idx-1);
                    }

                    if( x < width - 1 && y < height -1 )
                    {
                        myLines.push_back(idx);
                        myLines.push_back(idx+width + 1);
                    }
                    break;
                case 2:
                    if( x > 0 )
                    {
                        myLines.push_back(idx);
                        myLines.push_back(idx-width);
                    }
                    if( y < height - 1 )
                    {
                        myLines.push_back(idx);
                        myLines.push_back(idx+width);
                    }
                    break;
                }
            }
        }

    }
}

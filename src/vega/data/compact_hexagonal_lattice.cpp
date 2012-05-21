#include "compact_hexagonal_lattice.h"

using namespace vega;


void vega::data::compact_hexagonal_lattice::insert_lattice_line(uint32 idx1, uint32 idx2, uint16 delta_z)
{
	myLines.push_back(idx1);
	myLines.push_back(idx2);

	if( delta_z != 0 )
	{
		myLines.push_back(idx1);
		myLines.push_back(idx1+delta_z);

		myLines.push_back(idx2);
		myLines.push_back(idx2+delta_z);
	}
}

void vega::data::compact_hexagonal_lattice::create( uint16 width, uint16 height, uint16 depth, float span )
{
	for(uint16 z=0; z<depth; ++z)
	{
		float fz = z * span;
		uint16 diz = z < depth-1 ? width * height : 0;

		for(uint16 y=0; y<height; ++y)
		{
			float dx = y % 2 == 0 ? 0 : span / 2;
			float fy = y * span;
			
			for(uint16 x=0; x<width; ++x)
			{
				uint32 idx = z * width * height + y * width + x;

				myLattice.push_back(math::vector3d(x * span + dx, fy, fz));
				if( y % 2 == 0 )
				{
					if( x % 3 == 2 )
					{
						insert_lattice_line(idx, idx-1, diz);
					}
				}
				else
				{
					switch( x % 3 )
					{
					case 0:
						if( x < width-1 && y > 0 )
						{
							insert_lattice_line(idx, idx-width + 1, diz);
						}

						if( x > 0 )
						{
							insert_lattice_line(idx, idx-1, diz);
						}

						if( x < width - 1 && y < height -1 )
						{
							insert_lattice_line(idx, idx+width + 1, diz);
						}
						break;
					case 2:
						if( x > 0 )
						{
							insert_lattice_line(idx, idx-width, diz);
						}
						if( y < height - 1 )
						{
							insert_lattice_line(idx, idx+width, diz);
						}
						break;
					}
				}
			}
		}
	}
}

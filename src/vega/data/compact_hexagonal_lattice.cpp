#include "compact_hexagonal_lattice.h"


vega::data::compact_hexagonal_lattice::compact_hexagonal_lattice( uint16 width, uint16 height, uint16 depth, float span )
    : myWidth(width)
    , myHeight(height)
    , myDepth(depth)
    , mySpan(span)
{

}

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

bool vega::data::compact_hexagonal_lattice::create()
{
	for(uint16 z=0; z<myDepth; ++z)
	{
		float fz = z * mySpan;
		uint16 diz = z < myDepth-1 ? myWidth * myHeight : 0;

		for(uint16 y=0; y<myHeight; ++y)
		{
			float dx = y % 2 == 0 ? 0 : mySpan / 2;
			float fy = y * mySpan;
			
			for(uint16 x=0; x<myWidth; ++x)
			{
				uint32 idx = z * myWidth * myHeight + y * myWidth + x;

				myLattice.push_back(math::vector3d(x * mySpan + dx, fy, fz));
				if( y % 2 == 0 )
				{
					if( x % 3 == 2 )
					{
						insert_lattice_line(idx, idx - 1, diz);
					}
				}
				else
				{
					switch( x % 3 )
					{
					case 0:
						if( x < myWidth-1 && y > 0 )
						{
							insert_lattice_line(idx, idx - myWidth + 1, diz);
						}

						if( x > 0 )
						{
							insert_lattice_line(idx, idx-1, diz);
						}

						if( x < myWidth - 1 && y < myHeight -1 )
						{
							insert_lattice_line(idx, idx + myWidth + 1, diz);
						}
						break;
					case 2:
						if( x > 0 )
						{
							insert_lattice_line(idx, idx - myWidth, diz);
						}
						if( y < myHeight - 1 )
						{
							insert_lattice_line(idx, idx + myWidth, diz);
						}
						break;
					}
				}
			}
		}
	}

    return true;
}

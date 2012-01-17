#include "hexagonal_prismatic_lattice.h"
#include <stack>
#include <map>
#include <iostream>

using namespace vega;
using namespace vega::math;


vega::graph::hexagonal_prismatic_lattice::prismatic_hexagon_node::prismatic_hexagon_node( uint16 _x, uint16 _y, uint16 _z )
    : x(_x), y(_y), z(_z)
{
}

vega::graph::hexagonal_prismatic_lattice::prismatic_hexagon_node::~prismatic_hexagon_node()
{
}

//////////////////////////////////////////////////////////////////////////

vega::graph::hexagonal_prismatic_lattice::hexagonal_prismatic_lattice( const data::volume &v )
{
    std::stack<prismatic_hexagon_node*> st;
    std::map<unsigned int, prismatic_hexagon_node*> m;

    std::cout << "Creating hexagonal prismatic lattice..." << std::endl;

    myRoot = new prismatic_hexagon_node(0, 0, 0);
    st.push(myRoot);
    m[0] = myRoot;

#ifdef _DEBUG
	if( std::max( std::max(v.get_width(), v.get_height()), v.get_depth()) > 1024)
	{
		std::cerr << "WARNING: hexagonal_prismatic_lattice hash code will have conflicts !" << std::endl;
	}
#endif

    while( !st.empty() )
    {
        prismatic_hexagon_node * node = st.top();
        st.pop();

        static const int hex_voxel_offset[8][2] = { {1, 0}, {2, 0}, {0, 1}, {1, 1}, {2, 1}, {3, 1}, {1, 2}, {2, 2}};

        float fAvgDensity = 0.f;

        // compute average color
        for(size_t h=0;h<16;++h)
        {
            fAvgDensity += v.get_voxel(node->x + hex_voxel_offset[h%8][0], node->y + hex_voxel_offset[h%8][1], node->z + h/8);
        }

        // average on 16 voxels
        fAvgDensity *= 0.0625f;
        node->density = fAvgDensity;

		static const int hex_neighbor_offset[6+7+7][3] = 
		{ 
			{2, 1, 0}, {0, 2, 0}, {-2, 1, 0}, {-2, -1, 0}, {0, -2, 0}, {2, -1, 0},
			{0, 0, -1}, {2, 1, -1}, {0, 2, -1}, {-2, 1, -1}, {-2, -1, -1}, {0, -2, -1}, {2, -1, -1},
			{0, 0, 1}, {2, 1, 1}, {0, 2, 1}, {-2, 1, 1}, {-2, -1, 1}, {0, -2, 1}, {2, -1, 1}
		};

		// create neighbors
		for(int h=0; h<20; h++)
		{
			int xx = node->x + hex_neighbor_offset[h][0];
			int yy = node->y + hex_neighbor_offset[h][1];
			int zz = node->z + hex_neighbor_offset[h][2];

			if( node->hex[h] == NULL )
			{
				// if we are on the range
				if(xx >= 0 && xx + 3 < (int)v.get_width() && yy >= 0 && yy + 2 < (int)v.get_height() && zz >= 0 && zz + 1 < (int)v.get_depth())
				{
					unsigned int id = (xx << 20) | (yy << 10) | zz; // 10 bit code

					auto e = m.find(id);
					if( e == m.end() )
					{
						node->hex[h] = new prismatic_hexagon_node(xx, yy, zz);
						m[id] = node->hex[h];
						st.push(node->hex[h]);
					}
					else
					{
						node->hex[h] = e->second;
					}
				}
			}

			static const int link_center[6] = {3, 4, 5, 0, 1, 2};
			static const int link_circular_fw[6] = { 2, 3, 4, 5, 0, 1 };
			static const int link_circular_bk[6] = { 4, 5, 0, 1, 2, 3 };

			// -1 = node, -2 = NULL
			static const int link_up[20] = { 14, 15, 16, 17, 18, 19, -1, 0, 1, 2, 3, 4, 5, -2, -2, -2, -2, -2, -2, -2};
			static const int link_down[20] = { 7, 8, 9, 10, 11, 12, -2, -2, -2, -2, -2, -2, -2, -1, 0, 1, 2, 3, 4, 5 };

			// link neighbors
			for(int h=0; h<20; ++h)
			{
				if( node->hex[h] == NULL )
					continue;

				// link to center
				node->hex[h]->hex[link_center[h]] = node;

				int next = h == 5 ? 0 : h+1;
				int prev = h == 0 ? 5 : h-1;

				// link circle
				node->hex[h]->hex[link_circular_fw[h]] = node->hex[next];
				node->hex[h]->hex[link_circular_bk[h]] = node->hex[prev];

				// link up/down
				if( link_down[h] != -2 )
					node->hex[h]->hex[6] = link_down[h] == -1 ? node : node->hex[link_down[h]];
			}

		}
    }
}

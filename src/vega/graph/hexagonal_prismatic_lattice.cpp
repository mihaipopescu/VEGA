#include "hexagonal_prismatic_lattice.h"
#include <stack>
#include <map>
#include <list>
#include <iostream>
#include <algorithm>

using namespace vega;
using namespace vega::math;


vega::graph::hexagonal_prismatic_lattice::prismatic_hexagon_node::prismatic_hexagon_node( uint16 _x, uint16 _y, uint16 _z )
    : x(_x), y(_y), z(_z)
	, density(-1.f)
	, visited(false)
	, to_be_deleted(false)
{
	memset(hex, NULL, 20*sizeof(prismatic_hexagon_node*));
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
        float fMaxDeviation = 0.f;

        // compute average color
        for(size_t h=0;h<8;++h)
        {
            float vh = v.get_voxel(node->x + hex_voxel_offset[h][0], node->y + hex_voxel_offset[h][1], node->z);
            fAvgDensity += vh;
            for(size_t g=0; g<8; ++g)
            {
                float vg = v.get_voxel(node->x + hex_voxel_offset[g][0], node->y + hex_voxel_offset[g][1], node->z);
                float fDeviation = fabs(vh - vg);
                if( fDeviation > fMaxDeviation )
                    fMaxDeviation = fDeviation;
            }
        }

        // average on 8 voxels
        fAvgDensity *= 0.125f;
        node->density = fAvgDensity;
        node->max_density_deviation = fMaxDeviation;

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
		}

		static const int neighbor_map[20][20] = 
		{
			{-1, -1, 1, -2, 5, -1, 7, -1, -1, 8, 6, 12, -1, 14, -1, -1, 15, 13, 19, -1},
			{-1, -1, -1, 2, -2, 0, 8, -1, -1, -1, 9, 6, 7, 15, -1, -1, -1, 16, 13, 14},
			{1, -1, -1, -1, 3, -2, 9, 8, -1, -1, -1, 10, 6, 16, 15, -1, -1, -1, 17, 13},
			{-2, 2, -1, -1, -1, 4, 10, 6, 9, -1, -1, -1, 11, 17, 13, 16, -1, -1, -1, 18},
			{5, -2, 3, -1, -1, -1, 11, 12, 6, 10, -1, -1, -1, 18, 19, 13, 17, -1, -1, -1},
			{-1, 0, -2, 4, -1, -1, 12, -1, 7, 6, 11, -1, -1, 19, -1, 14, 13, 18, -1, -1},
			{7, 8, 9, 10, 11, 12, -1, -1, -1, -1, -1, -1, -1, -2, 0, 1, 2, 3, 4, 5},
			{-1, -1, 8, 6, 12, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, 1, -2, 5, -1},
			{-1, -1, -1, 9, 6, 7, -1, -1, -1, -1, -1, -1, -1, 1, -1, -1, -1, 2, -2, 0},
			{8, -1, -1, -1, 10, 6, -1, -1, -1, -1, -1, -1, -1, 2, 1, -1, -1, -1, 3, -2},
			{6, 9, -1, -1, -1, 11, -1, -1, -1, -1, -1, -1, -1, 3, -2, 2, -1, -1, -1, 4},
			{12, 6, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 4, 5, -2, 3, -1, -1, -1},
			{-1, 7, 6, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, 5, -1, 0, -2, 4, -1, -1},
			{14, 15, 16, 17, 18, 19, -2, 0, 1, 2, 3, 4, 5, -1, -1, -1, -1, -1, -1, -1},
			{-1, -1, 15, 13, 19, -1, 0, -1, -1, 1, -2, 5, -1, -1, -1, -1, -1, -1, -1, -1},
			{-1, -1, -1, 16, 13, 14, 1, -1, -1, -1, 2, -2, 0, -1, -1, -1, -1, -1, -1, -1},
			{15, -1, -1, -1, 17, 13, 2, 1, -1, -1, -1, 3, -2, -1, -1, -1, -1, -1, -1, -1},
			{13, 16, -1, -1, -1, 18, 3, -2, 2, -1, -1, -1, 4, -1, -1, -1, -1, -1, -1, -1},
			{19, 13, 17, -1, -1, -1, 4, 5, -2, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
			{-1, 14, 13, 18, -1, -1, 5, -1, 0, -2, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1}
		};

		// link neighbors
		for(int h=0; h<20; ++h)
		{
			if( node->hex[h] == NULL )
				continue;

			for(int i=0; i<20; ++i)
			{
				if( neighbor_map[h][i] != -1 )
				{
					node->hex[h]->hex[i] = neighbor_map[h][i] != -2 ? node->hex[neighbor_map[h][i]] : node;
				}
			}
		} //end-for
    } // end-while
}

vega::graph::hexagonal_prismatic_lattice::~hexagonal_prismatic_lattice()
{
	std::stack<prismatic_hexagon_node*> st;
	st.push(myRoot);

	static const int link_center[20] = {3, 4, 5, 0, 1, 2, 13, 17, 18, 19, 14, 15, 16, 6, 10, 11, 12, 7, 8, 9};
	
	while( !st.empty() )
	{
		prismatic_hexagon_node* node = st.top();
		st.pop();

		for(uint8 h=0; h<20; ++h)
		{
			if( node->hex[h] )
			{
				if( !node->hex[h]->to_be_deleted )
				{
					st.push(node->hex[h]);
					node->hex[h]->to_be_deleted = true;
				}
				node->hex[h]->hex[link_center[h]] = NULL;
			}
		}

		delete node;
	}
}

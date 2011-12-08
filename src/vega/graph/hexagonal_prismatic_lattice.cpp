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




    }
}

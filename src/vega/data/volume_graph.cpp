#include "volume_graph.h"
#include "../graph/hexagonal_prismatic_lattice.h"
#include <iostream>
#include <stack>

using namespace vega;
using namespace vega::math;

bool vega::data::volume_graph::create( const volume& v )
{
    volume::create(v);

	graph::hexagonal_prismatic_lattice * hpl = new graph::hexagonal_prismatic_lattice(v);

	std::cout << "Generating volume graph ..." << std::endl;

	std::stack<graph::hexagonal_prismatic_lattice::prismatic_hexagon_node*> st;
    st.push( hpl->get_root() );

    while( !st.empty() )
    {
        graph::hexagonal_prismatic_lattice::prismatic_hexagon_node* node = st.top();
        
        // visit the node
        st.pop();
        node->visited = true;

        node->vertexId = add_vertex(vector3d(node->x, node->y, node->z));

		(node->x+1, node->y+1, node->z) *= 10;

        // expand the list
        for(int i=0; i<20; ++i)
        {
            if( node->hex[i] )
            {
                if( !node->hex[i]->visited )
                {
                    st.push(node->hex[i]);
                }
                else
                {
                    add_edge(node->vertexId, node->hex[i]->vertexId, fabs(node->density - node->hex[i]->density));
                }
            }
        }
    }

	std::cout << "Cleaning up hexagonal prismatic lattice ..." << std::endl;
	delete hpl;

    return true;
}

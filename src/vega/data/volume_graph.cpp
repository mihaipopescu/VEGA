#include "volume_graph.h"
#include "../data/hexagonal_prismatic_lattice.h"
#include <iostream>
#include <stack>

using namespace vega::data;
using namespace vega::math;

bool vega::data::volume_graph::create( const volume& v )
{
    volume::create(v);

    uint16 w = get_width();
    uint16 h = get_height();
    uint16 d = get_depth();

	hexagonal_prismatic_lattice * hpl = new hexagonal_prismatic_lattice(&v);

	std::cout << "Generating volume graph ..." << std::endl;

	std::stack<hexagonal_prismatic_lattice::prismatic_hexagon_node*> st;
    st.push( hpl->get_root() );

	
    while( !st.empty() )
    {
        hexagonal_prismatic_lattice::prismatic_hexagon_node* node = st.top();
        
        // visit the node
        st.pop();
        node->visited = true;

        node->vertexId = add_vertex(vector3d((float)node->x / w - 0.5f, (float)node->y / h - 0.5f,  (float)node->z / d - 0.5f));

		//set_voxel(node->x+1, node->y+1, node->z, get_voxel(node->x+1, node->y+1, node->z) * MAXDENSITY * 10);

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


bool vega::data::volume_graph::create( const volume_base* v )
{
    //volume::create(v);

    uint16 w = v->get_width();
    uint16 h = v->get_height();
    uint16 d = v->get_depth();

    hexagonal_prismatic_lattice * hpl = new hexagonal_prismatic_lattice(v);

    std::cout << "Generating volume graph ..." << std::endl;

    std::stack<hexagonal_prismatic_lattice::prismatic_hexagon_node*> st;
    st.push( hpl->get_root() );


    while( !st.empty() )
    {
        hexagonal_prismatic_lattice::prismatic_hexagon_node* node = st.top();

        // visit the node
        st.pop();
        node->visited = true;

        node->vertexId = add_vertex(vector3d((float)node->x / w - 0.5f, (float)node->y / h - 0.5f,  (float)node->z / d - 0.5f));

        //set_voxel(node->x+1, node->y+1, node->z, get_voxel(node->x+1, node->y+1, node->z) * MAXDENSITY * 10);

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
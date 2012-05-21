#include <iostream>
#include "bitmap_graph.h"
#include "../math/transformations.h"
#include "../data/hexagonal_lattice.h"
#include <stack>
#include <iostream>


using namespace vega::data;
using namespace vega::math;

void vega::data::bitmap_graph::create( const image & img )
{
    image::create(img);

    hexagonal_lattice * hl = new hexagonal_lattice(img);

    std::cout << "Generating bitmap graph..." << std::endl;
    
    std::stack<hexagonal_lattice::hexagon_node*> st;
    st.push( hl->get_root() );

    while( !st.empty() )
    {
        hexagonal_lattice::hexagon_node * node = st.top();
        
        // visit the node
        st.pop();
        node->visited = true;

        node->vertexId = add_vertex(vector2d(node->x, node->y));

        image::set_pixel_contrast(node->x+1, node->y+1, 10.f);

        // expand the list
        for(int i=0; i<6; ++i)
        {
            if( node->hex[i] )
            {
                if( !node->hex[i]->visited )
                {
                    st.push(node->hex[i]);
                }
                else
                {
                    add_edge(node->vertexId, node->hex[i]->vertexId, HSV_distance_from_RGB(node->color, node->hex[i]->color));
                }
            }
        }
    }
    
    std::cout << "Cleaning up after creating bitmap graph..." << std::endl;
    delete hl;
}

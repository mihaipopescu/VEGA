#include "hexagonal_lattice.h"
#include "../math/transformations.h"

#include <stack>
#include <map>
#include <iostream>

using namespace vega;
using namespace vega::math;


vega::data::hexagonal_lattice::hexagon_node::hexagon_node( uint16 _x, uint16 _y )
    : x(_x)
    , y(_y)
    , visited(false)
	, to_be_deleted(false)
    , vertexId(size_t(-1))
{
     memset(hex, NULL, 6*sizeof(hexagon_node*));
}

vega::data::hexagonal_lattice::hexagon_node::~hexagon_node()
{
}

//////////////////////////////////////////////////////////////////////////

vega::data::hexagonal_lattice::hexagonal_lattice( const data::image & img )
{
    std::stack<hexagon_node*> st;
    std::map<unsigned int, hexagon_node*> m;

    std::cout << "Creating hexagonal lattice..." << std::endl;

    // init stack and map with root node
    myRoot = new hexagonal_lattice::hexagon_node(0, 0);
    st.push(myRoot);
    m[0] = myRoot;

    while( !st.empty() )
    {
        hexagon_node* node = st.top();
        st.pop();

        // the hexagon pixel offsets from the upper left corner
        // 
        // x 1-2 o
        //  /   \
        // 3-4-5-6
        //  \   /
        // o 7-8 o
        //

        static const int hex_pixel_offset[8][2] = { {1, 0}, {2, 0}, {0, 1}, {1, 1}, {2, 1}, {3, 1}, {1, 2}, {2, 2}};

        vector3d & rgb = node->color;

        // compute average color
        for(size_t h=0;h<8;++h)
        {
            rgb += img.get_pixel(node->x + hex_pixel_offset[h][0], node->y + hex_pixel_offset[h][1]);
        }

        // average on 8 pixels
        rgb = rgb * 0.125f;

        static const int hex_neighbor_offset[6][2] = { {2, 1}, {0, 2}, {-2, 1}, {-2, -1}, {0, -2}, {2, -1} };

        // create neighbors
        for(uint8 h=0; h<6; ++h)
        {
            int xx = (int)node->x + hex_neighbor_offset[h][0];
            int yy = (int)node->y + hex_neighbor_offset[h][1];

            if( node->hex[h] == NULL )
            {
                // if we are on the range
                if(xx >= 0 && xx + 3 < (int)img.get_width() && yy >= 0 && yy + 2 < (int)img.get_height() )
                {
                    unsigned int id = (xx << 16) | yy;

                    auto e = m.find(id);
                    if( e == m.end() )
                    {
                        node->hex[h] = new hexagon_node(xx, yy);
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

        static const int link_center[6] = {3, 4, 5, 0, 1, 2};
        static const int link_circular_fw[6] = { 2, 3, 4, 5, 0, 1 };
        static const int link_circular_bk[6] = { 4, 5, 0, 1, 2, 3 };

        // link neighbors
        for(int h=0; h<6; ++h)
        {
            if( node->hex[h] == NULL )
                continue;

            // link to center
            node->hex[h]->hex[link_center[h]] = node;

            int next = h == 5 ? 0 : h+1;
            int prev = h == 0 ? 5 : h-1;

#ifdef _DEBUG
            if( node->hex[h]->hex[link_circular_fw[h]] && node->hex[h]->hex[link_circular_fw[h]] != node->hex[next] 
            || node->hex[h]->hex[link_circular_bk[h]] && node->hex[h]->hex[link_circular_bk[h]] != node->hex[prev])
            {
                _asm int 3;
            }
#endif
            // link circle
            node->hex[h]->hex[link_circular_fw[h]] = node->hex[next];
            node->hex[h]->hex[link_circular_bk[h]] = node->hex[prev];
        }
    }
}

vega::data::hexagonal_lattice::~hexagonal_lattice()
{
    std::stack<hexagon_node*> st;
    st.push(myRoot);

    static const int link_center[6] = {3, 4, 5, 0, 1, 2};

    while( !st.empty() )
    {
        hexagon_node* node = st.top();
        st.pop();

        for(uint8 h=0; h<6; ++h)
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

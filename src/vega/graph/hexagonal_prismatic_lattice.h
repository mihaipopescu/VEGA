#ifndef __VEGA_GRAPH_HEXAGONAL_PRISMATIC_LATTICE_H__
#define __VEGA_GRAPH_HEXAGONAL_PRISMATIC_LATTICE_H__

#include "hexagonal_lattice.h"
#include "../data/volume.h"

namespace vega
{
    namespace graph
    {
        class hexagonal_prismatic_lattice
        {
        public:
            struct prismatic_hexagon_node
            {
            public:
                //     z=0           z=-1         z=+1
                //      __            __           __
                //   __/4 \__      __/11\__     __/18\__
                //  /3 \__/5 \    /10\__/12\   /17\__/19\
                //  \__/* \__/    \__/6 \__/   \__/13\__/
                //  /2 \__/0 \    /9 \__/7 \   /16\__/14\
                //  \__/1 \__/    \__/8 \__/   \__/15\__/
                //     \__/          \__/         \__/
                //
                //
                prismatic_hexagon_node(uint16 _x, uint16 _y, uint16 _z);
                ~prismatic_hexagon_node();

                float density;                              //!< average density
                uint16 x, y, z;                             //!< node coordinates
                prismatic_hexagon_node * hex[20];           //!< neighbors
				bool visited;								//!< visited flag
            };

            hexagonal_prismatic_lattice(const data::volume &v);
            ~hexagonal_prismatic_lattice();

        private:
            prismatic_hexagon_node * myRoot;
        };
    }
}


#endif // __VEGA_GRAPH_HEXAGONAL_PRISMATIC_LATTICE_H__
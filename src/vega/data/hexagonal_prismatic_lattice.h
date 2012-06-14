#ifndef __VEGA_DATA_HEXAGONAL_PRISMATIC_LATTICE_H__
#define __VEGA_DATA_HEXAGONAL_PRISMATIC_LATTICE_H__

#include "volume.h"

namespace vega
{
    namespace data
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
                float max_density_deviation;
                uint16 x, y, z;                             //!< node coordinates
                prismatic_hexagon_node * hex[20];           //!< neighbors
                bool visited;                               //!< visited flag
                bool to_be_deleted;                         //!< deleted flag
                size_t vertexId;
            };

            hexagonal_prismatic_lattice(const volume_base* v);
            ~hexagonal_prismatic_lattice();

            prismatic_hexagon_node* get_root() const { return myRoot; }

        private:
            prismatic_hexagon_node * myRoot;
        };
    }
}


#endif // __VEGA_DATA_HEXAGONAL_PRISMATIC_LATTICE_H__
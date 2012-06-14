#ifndef __VEGA_DATA_HEXAGONAL_LATTICE_H__
#define __VEGA_DATA_HEXAGONAL_LATTICE_H__

#include "image.h"
#include "../common/types.h"
#include "../math/vector3d.h"

namespace vega
{
    namespace data
    {
        class hexagonal_lattice
        {
        public:
            struct hexagon_node
            {

                //      __
                //   __/4 \__
                //  /3 \__/5 \
                //  \__/  \__/
                //  /2 \__/0 \
                //  \__/1 \__/
                //     \__/
                //

                hexagon_node(uint16 _x, uint16 _y);
                ~hexagon_node();

                uint16 x, y;            //!< insertion point 
                math::vector3d color;   //!< average color
                hexagon_node* hex[6];   //!< neighbor hexagons
                bool visited;
				bool to_be_deleted;
                size_t vertexId;
            };

        public:
            hexagonal_lattice(const image & img);
            ~hexagonal_lattice();

            hexagon_node* get_root() { return myRoot; }

        private:
            hexagon_node * myRoot;
        };
    }
}

#endif // __VEGA_DATA_HEXAGONAL_LATTICE_H__
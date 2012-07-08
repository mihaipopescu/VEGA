#ifndef __VEGA_DATA_HEXAGONAL_LATTICE_H__
#define __VEGA_DATA_HEXAGONAL_LATTICE_H__

#include <vector>
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

                uint16 x, y;            //!< insertion point
                math::vector3d color;   //!< average color
                uint32 hex[6];          //!< neighbor hexagons lattice index
                uint32 vertexId;        
            };

        public:
            hexagonal_lattice(const image & img);
        
            std::vector<hexagon_node> myLatticeCells;
            uint16 myWidth;
            uint16 myHeight;
        };
    }
}

#endif // __VEGA_DATA_HEXAGONAL_LATTICE_H__
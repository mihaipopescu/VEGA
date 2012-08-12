#ifndef __VEGA_DATA_HEXAGONAL_PRISMATIC_LATTICE_H__
#define __VEGA_DATA_HEXAGONAL_PRISMATIC_LATTICE_H__

#include <vector>
#include "volume.h"

#define NEIGHBOR_COUNT 8 // 20

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

                //     z=0           z=-1         z=+1
                //      __            __           __
                //   __/4 \__      __/  \__     __/  \__
                //  /3 \__/5 \    /  \__/  \   /  \__/  \
                //  \__/* \__/    \__/6 \__/   \__/7 \__/
                //  /2 \__/0 \    /  \__/  \   /  \__/  \
                //  \__/1 \__/    \__/  \__/   \__/  \__/
                //     \__/          \__/         \__/
                //
                //

                float density;                              //!< average density
                uint16 x, y, z;                             //!< node coordinates
                uint32 hex[NEIGHBOR_COUNT];                 //!< neighbors

                math::vector3d get_vertex() const;
            };

            hexagonal_prismatic_lattice(const volume& v);

        public:
            std::vector<prismatic_hexagon_node> myLatticeCells;

            uint16 myWidth;
            uint16 myHeight;
            uint16 myDepth;
        };
    }
}


#endif // __VEGA_DATA_HEXAGONAL_PRISMATIC_LATTICE_H__
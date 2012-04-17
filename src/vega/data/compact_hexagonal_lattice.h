#ifndef __VEGA_DATA_COMPACT_HEXAGONAL_LATTICE_H__
#define __VEGA_DATA_COMPACT_HEXAGONAL_LATTICE_H__

#include <vector>
#include "../math/vector2d.h"
#include "../common/types.h"
#include "common.h"

namespace vega
{
    namespace data
    {

        class compact_hexagonal_lattice : public i_model
        {
        public:
            compact_hexagonal_lattice() { }
            virtual ~compact_hexagonal_lattice() { }

            void create(uint8 width, uint8 height, float span);

        
            std::vector<math::vector2d> myLattice;
            std::vector<uint16> myLines;
        };


    }
}




#endif // __VEGA_DATA_COMPACT_HEXAGONAL_LATTICE_H__
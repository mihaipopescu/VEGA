#ifndef __VEGA_DATA_COMPACT_HEXAGONAL_LATTICE_H__
#define __VEGA_DATA_COMPACT_HEXAGONAL_LATTICE_H__

#include <vector>
#include "../math/vector3d.h"
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

            void create(uint8 width, uint8 height, uint8 depth, float span);

			void insert_lattice_line(uint32 idx, uint32 idx2, uint16 delta_z);
        
            std::vector<math::vector3d> myLattice;
            std::vector<uint32> myLines;
        };


    }
}




#endif // __VEGA_DATA_COMPACT_HEXAGONAL_LATTICE_H__
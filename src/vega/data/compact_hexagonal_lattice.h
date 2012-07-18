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
            compact_hexagonal_lattice(uint16 width, uint16 height, uint16 depth, float span);

            virtual bool create();

			void insert_lattice_line(uint32 idx, uint32 idx2, uint16 delta_z);
        
        public:
            std::vector<math::vector3d> myLattice;
            std::vector<uint32> myLines;

            uint16 myWidth;
            uint16 myHeight;
            uint16 myDepth;
            float mySpan;
        };
    }
}




#endif // __VEGA_DATA_COMPACT_HEXAGONAL_LATTICE_H__
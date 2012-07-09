#ifndef __VEGA_DATA_VOLUME_GRAPH_H__
#define __VEGA_DATA_VOLUME_GRAPH_H__

#include "common.h"
#include "volume.h"
#include "../data/graph.hpp"
#include "../math/vector3d.h"
#include "../data/hexagonal_prismatic_lattice.h"

#include <memory>

namespace vega
{
    namespace data
    {
        class volume_graph : public data::graph::weighted_undirected_graph<uint32>
        {
        public:
            volume_graph() {}

            void create(const volume& v);


        public:
            std::shared_ptr<hexagonal_prismatic_lattice> myLattice;
        };
    }
}


#endif // __VEGA_DATA_VOLUME_GRAPH_H__
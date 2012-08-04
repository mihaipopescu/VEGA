#ifndef __VEGA_DATA_VOLUME_GRAPH_H__
#define __VEGA_DATA_VOLUME_GRAPH_H__

#include "common.h"
#include "volume.h"
#include "../data/graph.hpp"
#include "../data/hexagonal_prismatic_lattice.h"

#include <memory>


namespace vega
{
    namespace data
    {
        class volume_graph : public volume, public data::graph::weighted_undirected_graph<uint32>
        {
        public:
            volume_graph(const std::string& _FileName);

            virtual bool create();
            
            void mst_kruskal();

        public:
            std::shared_ptr<hexagonal_prismatic_lattice> myLattice;
            std::vector<uint32> mySet;
        };
    }
}


#endif // __VEGA_DATA_VOLUME_GRAPH_H__
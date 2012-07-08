#ifndef __VEGA_DATA_BITMAP_GRAPH_H__
#define __VEGA_DATA_BITMAP_GRAPH_H__

#include <memory>
#include "image.h"
#include "../data/graph.hpp"
#include "hexagonal_lattice.h"


namespace vega
{
    namespace data
    {
        class bitmap_graph : protected data::graph::weighted_undirected_graph<uint32>
        {
        public:
            void create(const image & img);

        protected:
            std::shared_ptr<hexagonal_lattice> myLattice;
        };
    }
}

#endif
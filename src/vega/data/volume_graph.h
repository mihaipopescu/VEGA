#ifndef __VEGA_DATA_VOLUME_GRAPH_H__
#define __VEGA_DATA_VOLUME_GRAPH_H__

#include "common.h"
#include "volume.h"
#include "../data/hexagonal_prismatic_lattice.h"
#include "boost/graph/adjacency_list.hpp"
#include <memory>


namespace vega
{
    namespace data
    {
        class volume_graph : public volume
        {   
            typedef boost::adjacency_list < boost::vecS, boost::vecS, boost::undirectedS,
                boost::no_property, boost::property < boost::edge_weight_t, float > > Graph;
        public:
            volume_graph(const std::string& _FileName);

            virtual bool create();
            
            void mst_kruskal();

        public:
            std::shared_ptr<Graph> myGraph;
            std::shared_ptr<hexagonal_prismatic_lattice> myLattice;
            std::vector<uint32> mySet;
        };
    }
}


#endif // __VEGA_DATA_VOLUME_GRAPH_H__
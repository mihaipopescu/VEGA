#include "volume_graph.h"
#include "../common/logger.h"
#include "../algorithm/mst/kruskal.hpp"
#include "boost/pending/disjoint_sets.hpp"

#include <iostream>

using namespace boost;
using namespace vega::data;
using namespace vega::math;


vega::data::volume_graph::volume_graph( const std::string& _FileName )
    : volume(_FileName, false)
{
}

bool vega::data::volume_graph::create()
{
    SMART_LOG_FN;

    volume::create();

    myLattice = std::make_shared<hexagonal_prismatic_lattice>(*this);

    myGraph = std::make_shared<Graph>(myLattice->myDepth * myLattice->myHeight * myLattice->myWidth);

    uint32 c = 0;
    for(int k=0;k<myLattice->myDepth;++k)
    {
        for(int i=0;i<myLattice->myHeight;++i)
        {
            for(int j=0;j<myLattice->myWidth;++j)
            {
                hexagonal_prismatic_lattice::prismatic_hexagon_node& node = myLattice->myLatticeCells[c];
                for(int h=0;h<20;++h)
                {
                    if( node.hex[h] != (uint32)-1 )
                    {
                        add_edge(c, node.hex[h], fabsf(node.density - myLattice->myLatticeCells[node.hex[h]].density), *myGraph);
                    }
                }

                c++;
            }
        }
    }

    mySet.reserve(num_edges(*myGraph));
    mySet.insert(mySet.begin(), num_vertices(*myGraph), 0);

    return true;
}

void vega::data::volume_graph::mst_kruskal()
{
    using namespace boost;

    std::vector<graph_traits<Graph>::edge_descriptor> mst;

    std::cout << "Applying Thresholded Kruskal MST... ";

	size_t count = mst.size();
    mst.resize(num_vertices(*myGraph) - 1);

    auto &&parent_set = vega::algorithm::mst::kruskal_threshold(*myGraph, mst.begin(), count, 0.1f);

	mySet.clear();
    mySet.insert(mySet.begin(), parent_set.begin(), parent_set.end());
	
    std::cout << count << " edges added !" << std::endl;
    
    auto weight = get(edge_weight, *myGraph);

	auto g = std::make_shared<Graph>(myLattice->myDepth * myLattice->myHeight * myLattice->myWidth);

    for(size_t i=0; i<count; ++i)
    {
        add_edge(mst[i].m_source, mst[i].m_target, get(weight, mst[i]), *g);
    }

	myGraph = g;

    std::cout << "Done !" << std::endl;

    notify();
}


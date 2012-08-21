#include "volume_graph.h"
#include "../common/logger.h"
#include "../algorithm/mst/kruskal.hpp"
#include "boost/pending/disjoint_sets.hpp"

#include <iostream>

using namespace boost;
using namespace vega::data;
using namespace vega::math;


vega::data::volume_graph::volume_graph( std::shared_ptr<volume>& v )
{
    myVolume = v;
}

bool vega::data::volume_graph::create()
{
    VEGA_LOG_FN;

    myLattice = std::make_shared<hexagonal_prismatic_lattice>(*myVolume);

    myGraph = std::make_shared<Graph>(myLattice->myDepth * myLattice->myHeight * myLattice->myWidth);

    uint32 c = 0;
    for(int k=0;k<myLattice->myDepth;++k)
    {
        for(int i=0;i<myLattice->myHeight;++i)
        {
            for(int j=0;j<myLattice->myWidth;++j)
            {
                hexagonal_prismatic_lattice::prismatic_hexagon_node& node = myLattice->myLatticeCells[c];
                for(int h=0;h<NEIGHBOR_COUNT;++h)
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

    mySet.reserve(num_vertices(*myGraph));
    mySet.insert(mySet.begin(), num_vertices(*myGraph), 0);

    return true;
}

void vega::data::volume_graph::mst_kruskal()
{
    using namespace boost;

    std::vector<graph_traits<Graph>::edge_descriptor> mst;

    VEGA_LOG_DEBUG("Applying Thresholded Kruskal MST... ");

	size_t count = mst.size();
    mst.resize(num_vertices(*myGraph) - 1);

    auto &&parent_set = vega::algorithm::mst::kruskal_threshold(*myGraph, mst.begin(), count, 0.1f);

	mySet.clear();
    mySet.insert(mySet.begin(), parent_set.begin(), parent_set.end());
	
    std::stringstream msg;
    msg << count << " edges added ! ";
    VEGA_LOG_DEBUG(msg.str().c_str());
    
    auto weight = get(edge_weight, *myGraph);

	auto g = std::make_shared<Graph>(myLattice->myDepth * myLattice->myHeight * myLattice->myWidth);

    for(size_t i=0; i<count; ++i)
    {
        add_edge(mst[i].m_source, mst[i].m_target, get(weight, mst[i]), *g);
    }

	myGraph = g;

    uint32 c = 0;
    for(auto it=myLattice->myLatticeCells.begin(); it != myLattice->myLatticeCells.end(); ++it)
    {
        myLattice->fill_volume_cell(*myVolume, *it);
    }
    
    myVolume->paint_voxels();

    VEGA_LOG_DEBUG("Done !\n");

    notify();
}


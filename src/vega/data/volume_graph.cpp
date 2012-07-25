#include "volume_graph.h"
#include "../common/logger.h"
#include "../data/graph.hpp"
#include "../algorithm/mst/kruskal.hpp"
#include "../data/disjoint_threshold_sets.hpp"
#include <iostream>

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

    uint32 c = 0;
    for(int k=0;k<myLattice->myDepth;++k)
    {
        for(int i=0;i<myLattice->myHeight;++i)
        {
            for(int j=0;j<myLattice->myWidth;++j)
            {
                add_vertex(c++);
            }
        }
    }

    c = 0;
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
                        add_edge(c, node.hex[h], fabsf(node.density - myLattice->myLatticeCells[node.hex[h]].density));
                }

                c++;
            }
        }
    }

    return true;
}

void vega::data::volume_graph::mst_kruskal()
{
    typedef std::vector<data::volume_graph::edge_type> MSTContainer;
    MSTContainer mst;

    std::cout << "Applying Thresholded Kruskal MST... ";

    std::vector<uint32> set;

    mst.resize(get_num_vertices() - 1);
    uint16 mst_size = vega::algorithm::mst::kruskal(*this, mst.begin(), 0.1f, set);

    std::cout << mst.size() << " edges added !" << std::endl;
    
    weighted_undirected_graph::clear_edges();

    for(int i=0; i<mst_size; ++i)
        weighted_undirected_graph::add_edge(mst[i].source, mst[i].target, mst[i].weight);

    std::cout << "Done !" << std::endl;

    notify();
}


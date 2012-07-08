#include "volume_graph.h"
#include "../common/logger.h"


using namespace vega::data;
using namespace vega::math;


void vega::data::volume_graph::create( const volume& v )
{
    SMART_LOG_FN;

    myLattice = std::make_shared<hexagonal_prismatic_lattice>(v);

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
}


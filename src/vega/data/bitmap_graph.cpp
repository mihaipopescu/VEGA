#include "bitmap_graph.h"
#include "../math/transformations.h"
#include "../common/logger.h"


void vega::data::bitmap_graph::create( const image & img )
{
    SMART_LOG_FN;

    myLattice = std::make_shared<hexagonal_lattice>(img);
    
    uint32 k = 0;
    for(int i=0;i<myLattice->myHeight;++i)
    {
        for(int j=0;j<myLattice->myWidth;++j)
        {
            myLattice->myLatticeCells[k++].vertexId = add_vertex(k);
        }
    }

    k=0;
    for(int i=0;i<myLattice->myHeight;++i)
    {
        for(int j=0;j<myLattice->myWidth;++j)
        {
            hexagonal_lattice::hexagon_node& node = myLattice->myLatticeCells[k++];
            for(int h=0;h<6;++h)
            {
                if( node.hex[h] != (uint32)-1 )
                    add_edge(node.vertexId, myLattice->myLatticeCells[node.hex[h]].vertexId, math::HSV_distance_from_RGB(node.color, myLattice->myLatticeCells[node.hex[h]].color));
            }
        }
    }
}

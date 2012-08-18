#ifndef __VEGA_DATA_VOLUME_GRAPH_H__
#define __VEGA_DATA_VOLUME_GRAPH_H__

#include "common.h"
#include "resizeable_volume.h"
#include "../data/hexagonal_prismatic_lattice.h"
#include "boost/graph/adjacency_list.hpp"
#include <memory>


namespace vega
{
    namespace data
    {
        class volume_graph : public i_volume
        {   
            typedef boost::adjacency_list < boost::vecS, boost::vecS, boost::undirectedS,
                boost::no_property, boost::property < boost::edge_weight_t, float > > Graph;
        public:
            volume_graph(std::shared_ptr<volume>& v);

            virtual bool create();
            
            virtual size_t get_size             () const { return myVolume->get_size(); }
            virtual uint16 get_width            () const { return myVolume->get_width(); }
            virtual uint16 get_height           () const { return myVolume->get_height(); }
            virtual uint16 get_depth            () const { return myVolume->get_depth(); }

            virtual void*  get_raw_data()       { return myVolume->get_raw_data(); }
            virtual void*  get_lut_data()       { return myVolume->get_lut_data(); }
            virtual void*  get_color_data()     { return myVolume->get_color_data(); }

            void mst_kruskal();

        public:
            std::shared_ptr<Graph> myGraph;
            std::shared_ptr<hexagonal_prismatic_lattice> myLattice;
            std::shared_ptr<volume> myVolume;
            std::vector<uint32> mySet;
        };
    }
}


#endif // __VEGA_DATA_VOLUME_GRAPH_H__
#ifndef __VEGA_DATA_VOLUME_TEXTURE_GRAPH_H__
#define __VEGA_DATA_VOLUME_TEXTURE_GRAPH_H__

#include "volume_texture.h"
#include "../graph/graph.hpp"
#include "../math/vector3d.h"


namespace vega
{
    namespace data
    {
        class volume_texture_graph : public graph::weighted_undirected_graph<math::vector3d>
        {
        public:
            volume_texture_graph();

            virtual bool create(const volume_base *v);
           
        };
    }
}



#endif // __VEGA_DATA_VOLUME_TEXTURE_GRAPH_H__
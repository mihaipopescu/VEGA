#ifndef __VEGA_DATA_VOLUME_GRAPH_H__
#define __VEGA_DATA_VOLUME_GRAPH_H__

#include "common.h"
#include "volume.h"
#include "../graph/graph.hpp"
#include "../math/vector3d.h"

namespace vega
{
    namespace data
    {
        class volume_graph : public volume, protected graph::weighted_undirected_graph<math::vector3d>
        {
        public:
            volume_graph() {}

            virtual bool create(const volume& v);

        };
    }
}


#endif // __VEGA_DATA_VOLUME_GRAPH_H__
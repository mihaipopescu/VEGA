#ifndef __VEGA_DATA_BITMAP_GRAPH_H__
#define __VEGA_DATA_BITMAP_GRAPH_H__

#include <memory>
#include "common.h"
#include "image.h"
#include "../math/vector2d.h"
#include "../graph/graph.hpp"


namespace vega
{
    namespace data
    {
        class bitmap_graph : public image, protected graph::weighted_undirected_graph<math::vector2d>
        {
        public:
            bitmap_graph() {}

            virtual void create(const image & img);

        protected:

        };
    }
}

#endif
#ifndef __VEGA_RENDER_GRAPH_VIEW__
#define __VEGA_RENDER_GRAPH_VIEW__

#include "common.h"
#include "../graph/graph.hpp"
#include "../math/vector2d.h"
#include "../math/vector3d.h"


namespace vega
{
	namespace render
	{

		class graph_view : public i_view
		{
		public:
			template <class VertexType>
			void create(const graph::weighted_undirected_graph<VertexType>& g);

			virtual void render() const;

		private:

		};


		class graph_presenter : public i_presenter
		{
		public:

		};
	}
}

#endif // __VEGA_RENDER_GRAPH_VIEW__
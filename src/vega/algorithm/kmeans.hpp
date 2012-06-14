#ifndef __VEGA_GRAPH_KMEANS_HPP__
#define __VEGA_GRAPH_KMEANS_HPP__

#include "../common/types.h"

namespace vega
{
	namespace algorithm
	{

		template <class Graph>
		class kmeans
		{
		public:
			kmeans(uint8 k) : myK(k) {}

		private:
			uint8 myK;
		};

	}
}


#endif // __VEGA_GRAPH_KMEANS_HPP__
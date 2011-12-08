#ifndef __VEGA_GRAPH_EDGE_HPP__
#define __VEGA_GRAPH_EDGE_HPP__

#include "common.hpp"


namespace vega
{
  namespace graph
  {
    template <class Graph>
    struct edge
    {
      typedef typename graph_traits<Graph>::vertex_id_type Vertex;

      inline edge() {}
      inline edge(Vertex s, Vertex t) : source(s), target(t) {}

      Vertex source;
      Vertex target;
      
	    bool operator==(const edge& e) const
	    {
		    return source == e.source && target == e.target || is_undirected<Graph>() && source == e.target && target == e.source;
	    }

	    bool operator!=(const edge &e) const
	    {
		    return !(*this == e);
	    }

    };

    template <class Graph, typename Weight = float>
    struct weighted_edge : public edge<Graph>
    {
      inline weighted_edge() {}
      inline weighted_edge(Vertex s, Vertex t, Weight w) : edge(s, t), weight(w) {}

      bool operator<(const weighted_edge& e) const
      {
        return weight < e.weight;
      }

      Weight weight;
    };


  }
}

#endif
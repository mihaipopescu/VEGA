#ifndef __VEGA_MST_KRUSKAL_HPP__
#define __VEGA_MST_KRUSKAL_HPP__

#include <algorithm>
#include "../graph/graph.hpp"
#include "../graph/disjoint_sets.hpp"
#include "../graph/disjoint_threshold_sets.hpp"


namespace vega
{
    namespace mst
    {
        using namespace graph;

        template <class Graph, class OutputIterator>
        void kruskal(const Graph& g, OutputIterator spanning_tree_edges)
        {
            disjoint_sets<Graph> dset(g.get_num_vertices());

            graph_traits<Graph>::edge_container_type edges = g.edges();

            std::sort(edges.begin(), edges.end());

            for(graph_traits<Graph>::edge_container_type::const_iterator it = edges.begin(); it != edges.end(); ++it)
            {
                if( !dset.find(it->source, it->target) )
                {
                    dset.link(it->source, it->target);
                    *spanning_tree_edges++ = *it;
                }
            }
        }

		template <class Graph, class OutputIterator>
		void kruskal2(const Graph& g, OutputIterator spanning_tree_edges, typename graph::graph_traits<Graph>::edge_weight_type treshold)
        {
            disjoint_treshold_sets<Graph> dset(g.get_num_vertices(), treshold);

            graph_traits<Graph>::edge_container_type edges = g.edges();

            std::sort(edges.begin(), edges.end());

            for(graph_traits<Graph>::edge_container_type::const_iterator it = edges.begin(); it != edges.end(); ++it)
            {
                if( !dset.find(it->source, it->target) )
                {
                    dset.link(it->source, it->target, it->weight);
                    *spanning_tree_edges++ = *it;
                }
            }
        }

    }
}

#endif
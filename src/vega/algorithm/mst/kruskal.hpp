#ifndef __VEGA_MST_KRUSKAL_HPP__
#define __VEGA_MST_KRUSKAL_HPP__

#include <algorithm>
#include "../../data/graph.hpp"
#include "../../data/disjoint_sets.hpp"
#include "../../data/disjoint_threshold_sets.hpp"


namespace vega
{
    namespace algorithm
    {
        namespace mst
        {
            using namespace vega::data::graph;

            template <class Graph, class OutputIterator>
            void kruskal(const Graph& g, OutputIterator spanning_tree_edges)
            {
                disjoint_sets<Graph> dset(g.get_num_vertices());

                graph_traits<Graph>::edge_container_type edges = g.edges();

                std::sort(edges.begin(), edges.end());

                for(graph_traits<Graph>::edge_container_type::const_iterator it = edges.begin(); it != edges.end(); ++it)
                {
                    if( dset.link(it->source, it->target) )
                    {
                        *spanning_tree_edges++ = *it;
                    }
                }
            }

            template <class Graph, class EdgeOutputIterator>
            disjoint_sets<Graph> kruskal(Graph& g, EdgeOutputIterator spanning_tree_edges, size_t &edge_count, typename data::graph::graph_traits<Graph>::edge_weight_type treshold)
            {
                disjoint_sets<Graph> dset(g.get_num_vertices());

                g.sort_edges_after_weight();

                edge_count = 0;
                for(graph_traits<Graph>::edge_container_type::const_iterator it = g.edges().begin(); it != g.edges().end(); ++it)
                {
                    if( it->weight < treshold )
                    {
                        if( dset.link(it->source, it->target) )
                        {
                            *spanning_tree_edges++ = *it;
                            ++edge_count;
                        }
                    }
                }

                return dset;
            }
        }
    }
}

#endif
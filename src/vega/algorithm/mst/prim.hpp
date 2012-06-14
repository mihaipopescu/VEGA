#ifndef __VEGA_MST_PRIM_HPP__
#define __VEGA_MST_PRIM_HPP__

#include "../../graph/graph.hpp"
#include "../../graph/disjoint_sets.hpp"


namespace vega
{
    namespace algorithm
    {
        namespace mst
        {
            using namespace vega::graph;

            template <class Graph, class OutputIterator>
            void prim(const Graph& g, OutputIterator spanning_tree_edges)
            {
                typedef graph_traits<Graph>::vertex_id_type vid_t;

                vid_t nVertices = g.get_num_vertices();

                std::vector<graph_traits<Graph>::edge_weight_type> wt(nVertices);
                std::vector<graph_traits<Graph>::edge_type> fr(nVertices);
                std::vector<vid_t> mst(nVertices);
                graph_traits<Graph>::edge_weight_type inf = infinite_weight<graph_traits<Graph>::edge_weight_type>();

                for(vid_t v = 0; v < nVertices; ++v)
                {
                    wt[v] = inf;
                    mst[v] = -1;
                }

                vid_t min = -1;
                for(vid_t v = 0; min != 0; v = min)
                {
                    min = 0;
                    for(vid_t w = 1; w < nVertices; ++w)
                    {
                        if( mst[w] == -1 )
                        {
                            graph_traits<Graph>::edge_container_type::const_iterator it;
                            if( g.get_edge(v, w, it) )
                            {
                                if( it->weight < wt[w] )
                                {
                                    wt[w] = it->weight;
                                    fr[w] = *it;
                                }
                            }

                            if( wt[w] < wt[min] )
                                min = w;
                        }
                    }

                    if( min != 0 )
                    {
                        *spanning_tree_edges++ = fr[min];
                        mst[min] = min;
                    }
                }
            }

        }
    }
}

#endif
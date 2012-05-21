#ifndef __VEGA_MST_BORUVKA_HPP__
#define __VEGA_MST_BORUVKA_HPP__

#include <algorithm>
#include "../graph/graph.hpp"
#include "../graph/disjoint_sets.hpp"

#include "../common/profiler.h"

namespace vega
{
    namespace algorithm
    {
        using namespace graph;

        template <class Graph, class OutputIterator>
        void boruvka(const Graph& g, OutputIterator spanning_tree_edges)
        {
            typedef graph_traits<Graph>::edge_type edge_t;
            typedef graph_traits<Graph>::vertex_id_type vid_t;

            edge_t z(0, 0, infinite_weight<graph_traits<Graph>::edge_weight_type>());

            PROFILE_SMART(1)
            disjoint_sets<vid_t> dset(g.get_num_vertices());

            std::vector<edge_t> a(g.get_num_edges());
            std::vector<edge_t> b(g.get_num_vertices());

            PROFILE_BEGIN(2)
            g.copy_edges(a.begin());
            PROFILE_END(2)

            PROFILE_BEGIN(3)
            vid_t N, k = 1;
            for (vid_t E = g.get_num_edges(); E != 0; E = N)
            {
                vid_t h, i, j;

                PROFILE_BEGIN(4)
                for (vid_t t = 0; t < g.get_num_vertices(); t++) 
                    b[t] = z;
                PROFILE_END(4)

                PROFILE_BEGIN(5)
                for (h = 0, N = 0; h < E; h++)
                {
                    const edge_t& e = a[h];
                    i = dset.find(e.source);
                    j = dset.find(e.target);
                    if (i == j) continue;
                    if (e.weight < b[i].weight)
                        b[i] = e;
                    if (e.weight < b[j].weight)
                        b[j] = e;
                    a[N++] = e;
                }
                PROFILE_END(5)

                PROFILE_BEGIN(6)
                for (h = 0; h < g.get_num_vertices(); h++)
                {
                    if (b[h] != z)
                    {
                        if (!dset.find(i = b[h].source, j = b[h].target))
                        {
                            dset.link(i, j);
                            *spanning_tree_edges++ = b[h];
                        }
                    }
                }
                PROFILE_END(6)
            }
            PROFILE_END(3)
        }
    }
}

#endif
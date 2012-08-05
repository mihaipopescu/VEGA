#ifndef __VEGA_MST_KRUSKAL_HPP__
#define __VEGA_MST_KRUSKAL_HPP__

#include <vector>
#include <queue>
#include <functional>

#include <boost/property_map/property_map.hpp>
#include <boost/graph/graph_concepts.hpp>
#include <boost/graph/named_function_params.hpp>
#include <boost/pending/disjoint_sets.hpp>
#include <boost/pending/indirect_cmp.hpp>
#include <boost/concept/assert.hpp>



namespace vega
{
    namespace algorithm
    {
        namespace mst
        {
            namespace detail
            {
                template <class Graph, class OutputIterator, 
                class Rank, class Parent, class Weight, class WeightTreshold>
                    void
                    kruskal_threshold(const Graph& G, 
                    OutputIterator spanning_tree_edges, 
                    Rank rank, Parent parent, Weight weight, WeightTreshold threshold)
                {
                    if (num_vertices(G) == 0) return; // Nothing to do in this case
                    typedef typename graph_traits<Graph>::vertex_descriptor Vertex;
                    typedef typename graph_traits<Graph>::edge_descriptor Edge;
                    BOOST_CONCEPT_ASSERT(( VertexListGraphConcept<Graph> ));
                    BOOST_CONCEPT_ASSERT(( EdgeListGraphConcept<Graph> ));
                    BOOST_CONCEPT_ASSERT(( OutputIteratorConcept<OutputIterator, Edge> ));
                    BOOST_CONCEPT_ASSERT(( ReadWritePropertyMapConcept<Rank, Vertex> ));
                    BOOST_CONCEPT_ASSERT(( ReadWritePropertyMapConcept<Parent, Vertex> ));
                    BOOST_CONCEPT_ASSERT(( ReadablePropertyMapConcept<Weight, Edge> ));
                    typedef typename property_traits<Weight>::value_type W_value;
                    typedef typename property_traits<Rank>::value_type R_value;
                    typedef typename property_traits<Parent>::value_type P_value;
                    BOOST_CONCEPT_ASSERT(( ComparableConcept<W_value> ));
                    BOOST_CONCEPT_ASSERT(( ConvertibleConcept<P_value, Vertex> ));
                    BOOST_CONCEPT_ASSERT(( IntegerConcept<R_value> ));

                    disjoint_sets<Rank, Parent>  dset(rank, parent);

                    typename graph_traits<Graph>::vertex_iterator ui, uiend;
                    for (boost::tie(ui, uiend) = vertices(G); ui != uiend; ++ui)
                        dset.make_set(*ui);

                    typedef indirect_cmp<Weight, std::greater<W_value> > weight_greater;
                    weight_greater wl(weight);

                    std::priority_queue<Edge, std::vector<Edge>, weight_greater> Q(wl);
                    /*push all edge into Q*/
                    typename graph_traits<Graph>::edge_iterator ei, eiend;
                    for (boost::tie(ei, eiend) = edges(G); ei != eiend; ++ei) 
                        Q.push(*ei);

                    while (! Q.empty()) {
                        Edge e = Q.top();
                        Q.pop();
                        Vertex u = dset.find_set(source(e, G));
                        Vertex v = dset.find_set(target(e, G));
                        
                        if ( u != v && get(weight, e) < threshold) {
                            *spanning_tree_edges++ = e;
                            dset.link(u, v);
                        }
                    }
                }

            } // namespace detail 

            // Named Parameters Variants

            template <class Graph, class OutputIterator, class Weight>
            inline std::vector<typename boost::graph_traits<Graph>::vertex_descriptor>
                kruskal_threshold(const Graph& g,
                OutputIterator spanning_tree_edges, Weight weight_threshold)
            {
                typedef typename graph_traits<Graph>::vertices_size_type size_type;
                typedef typename graph_traits<Graph>::vertex_descriptor vertex_t;
                typedef typename property_map<Graph, vertex_index_t>::type index_map_t;
                
                typename graph_traits<Graph>::vertices_size_type
                    n = num_vertices(g);
                std::vector<size_type> rank_map(n);
                std::vector<vertex_t> pred_map(n);

                if (num_vertices(g) == 0) return pred_map; // Nothing to do in this case

                detail::kruskal_threshold
                    (g, spanning_tree_edges, 
                    make_iterator_property_map(rank_map.begin(), get(vertex_index, g), rank_map[0]),
                    make_iterator_property_map(pred_map.begin(), get(vertex_index, g), pred_map[0]),
                    get(edge_weight, g), weight_threshold);

                return pred_map;
            }
        }
    }
}

#endif
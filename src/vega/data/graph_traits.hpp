#ifndef __VEGA_DATA_COMMON_HPP__
#define __VEGA_DATA_COMMON_HPP__


namespace vega
{
    namespace data
    {
        namespace graph
        {
            template <class G>
            struct graph_traits
            {
                typedef typename G::vertex_type            vertex_type;
                typedef typename G::edge_type              edge_type;

                typedef typename G::vertex_id_type         vertex_id_type;
                typedef typename G::edge_id_type           edge_id_type;

                typedef typename G::vertex_container_type  vertex_container_type;
                typedef typename G::edge_container_type    edge_container_type;

                typedef typename G::edge_weight_type       edge_weight_type;

                typedef typename G::directed_category      directed_category;
            };

            struct directed_tag { };
            struct undirected_tag { };

            namespace detail 
            {
                inline bool is_directed(directed_tag) { return true; }
                inline bool is_directed(undirected_tag) { return false; }
            }

            template <typename Graph>
            bool is_directed()
            {
                typedef typename graph_traits<Graph>::directed_category Cat;
                return detail::is_directed(Cat());
            }

            template <typename Graph>
            bool is_undirected()
            {
                return !is_directed<Graph>();
            }

            template <class T>
            T infinite_weight()
            {
                return (T)(0x80000000);
            }
        }
    }
}


#endif
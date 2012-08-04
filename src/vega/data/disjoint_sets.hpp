#ifndef __VEGA_DATA_DISJOINT_SETS_HPP__
#define __VEGA_DATA_DISJOINT_SETS_HPP__

#include <vector>


namespace vega 
{
    namespace data 
    {
        namespace graph 
        {
            template <class Graph>
            class disjoint_sets
            {
            public:
                typedef typename graph_traits<Graph>::vertex_id_type Index;
                typedef std::vector<Index> ParentContainer;
                typedef typename ParentContainer::size_type size_type;
                typedef std::vector<size_type> RankContainer;

                disjoint_sets(size_type n) : parent(n), rank(n, 0)
                {
                    for(Index i = 0; i < n; ++i)
                    {
                        parent[i] = i;
                        rank[i] = 1;
                    }
                }

                template <class Element>
                Element find(Element x)
                {
                    while (x != parent[x])
                        x = parent[x];
                    return x;
                }

                template <class Element>
                bool find(Element x, Element y)
                {
                    return find(x) == find(y);
                }

                template <class Element>
                bool link(Element x, Element y)
                {
                    Element i = find(x), j = find(y);

                    if (i==j)
                        return false;

                    if (rank[i] < rank[j])
                    {
                        parent[i] = j;
                        rank[j] += rank[i];
                    }
                    else
                    {
                        parent[j] = i;
                        rank[i] += rank[j];
                    }

                    return true;
                }

            public:
                ParentContainer parent;
                RankContainer rank;
            };
        }
    }
}

#endif // __VEGA_DATA_DISJOINT_SETS_HPP__
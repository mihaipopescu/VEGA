#ifndef __VEGA_GRAPH_DISJOINT_TRESHOLD_SETS_HPP__
#define __VEGA_GRAPH_DISJOINT_TRESHOLD_SETS_HPP__

#include <vector>


namespace vega {
  namespace graph {

    template <class Graph>
    class disjoint_treshold_sets
    {
    public:
	  typedef typename graph_traits<Graph>::vertex_id_type Index;
	  typedef typename graph_traits<Graph>::edge_weight_type Threshold;
      typedef std::vector<Index> ParentContainer;
      typedef typename ParentContainer::size_type size_type;
      typedef std::vector<size_type> RankContainer;
	  typedef std::vector<Threshold> ThresholdContainer;

      disjoint_treshold_sets(size_type n, Threshold th) 
		: parent(n), rank(n, 0), threshold(th), max_dist(n)
      {
        for(Index i = 0; i < n; ++i)
        {
          parent[i] = i;
          rank[i] = 1;
		  max_dist[i] = -1;
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
      void link(Element x, Element y, Threshold th)
      {
        Element i = find(x), j = find(y);

        if (i==j)
          return;
		
		Threshold mdi = max_dist[i] + threshold;
		Threshold mdj = max_dist[j] + threshold;
		
		Threshold md = max(mdi, mdj);
		if( md < th )
		{
			return;
		}

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
      }
    
    protected:
      ParentContainer parent;
      RankContainer rank;
	  ThresholdContainer max_dist;
	  Threshold threshold;
    };

  }
}
#endif
#ifndef __VEGA_GRAPH_GRAPH_HPP__
#define __VEGA_GRAPH_GRAPH_HPP__

#include "common.hpp"
#include "edge.hpp"
#include <vector>
#include <map>

namespace vega
{
  namespace graph
  {

    template <class Vertex>
    class weighted_undirected_graph
    {
    public:
      typedef typename weighted_undirected_graph self;

      typedef Vertex vertex_type;
      typedef std::size_t vertex_id_type;
      typedef typename std::vector<vertex_type> vertex_container_type;
      
      typedef float edge_weight_type;
      typedef typename weighted_edge<self, edge_weight_type> edge_type;
      typedef std::size_t edge_id_type;
      typedef typename std::vector<edge_type> edge_container_type;

      typedef undirected_tag directed_category;

    public:
      weighted_undirected_graph() {}

      vertex_id_type add_vertex(const vertex_type x)
      {
        myVertices.push_back(x);
        return myVertices.size()-1;
      }

      void clear_vertices()
      {
        myVertices.clear();
      }

      vertex_id_type get_num_vertices() const
      {
        return myVertices.size();
      }

      const vertex_container_type& vertices() const
      {
        return myVertices;
      }

      const void* get_vertices_data() const
      {
        return myVertices.data();
      }

      vertex_type get_vertex(vertex_id_type v) const
      {
        return myVertices[v];
      }

      void add_edge(vertex_id_type u, vertex_id_type v, edge_weight_type w)
      {
        myEdges.push_back(edge_type(u, v, w));
        myAdjacencyList[u].push_back(v);
        myAdjacencyList[v].push_back(u);
      }

      void get_adjacency(vertex_id_type u, std::vector<vertex_id_type>& adj) const
      {
          if( myAdjacencyList.find(u) != myAdjacencyList.end() )
          {
            adj = myAdjacencyList[u];
            return true;
          }
          return false;
      }

      bool get_edge(vertex_id_type u, vertex_id_type v, typename edge_container_type::const_iterator& e) const
      {
        for(edge_container_type::const_iterator it = myEdges.begin(); it!=myEdges.end(); ++it)
        {
          if( it->source == u && it->target == v || it->source == v && it->target == u )
          {
            e = it;
            return true;
          }
        }

        return false;
      }

      void remove_edge(vertex_id_type u, vertex_id_type v)
      {
        edge_container_type::const_iterator it;
        if( get_edge(u, v, it) )
          myEdges.erase(it);
      }

      template <class OutputIterator>
      void copy_edges(OutputIterator edges) const
      {
        for(edge_container_type::const_iterator it = myEdges.begin(); it!=myEdges.end(); ++it)
        {
          *edges++ = *it;
        }
      }

      void clear_edges()
      {
        myEdges.clear();
      }

      const edge_container_type& edges() const
      {
        return myEdges;
      }

      void sort_edges_after_weight()
      {
        std::sort(myEdges.begin(), myEdges.end());
      }
      
      vertex_id_type get_num_edges() const
      {
        return myEdges.size();
      }
      
    protected:
      vertex_container_type myVertices;
      edge_container_type myEdges;
      std::map<vertex_id_type, std::vector<vertex_id_type>> myAdjacencyList;
    };

  }
}


#endif
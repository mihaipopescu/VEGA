#include "graph_view.h"


void vega::render::graph_view::render() const
{

}


template <>
void vega::render::graph_view::create<vega::math::vector2d>( const vega::graph::weighted_undirected_graph<math::vector2d>& g )
{

}

template <>
void vega::render::graph_view::create<vega::math::vector3d>( const vega::graph::weighted_undirected_graph<math::vector3d>& g )
{

}
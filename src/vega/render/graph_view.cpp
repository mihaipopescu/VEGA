#include <windows.h>
#include <GL/GL.h>
#include <xutility>
#include <iostream>
#include "graph_view.h"
#include "../data/volume_graph.h"
#include "../graph/graph.hpp"
#include "../mst/kruskal.hpp"
#include "../graph/disjoint_threshold_sets.hpp"


void vega::render::graph_view::render() const
{
	if(myRenderFlag)
	{
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);
		glVertexPointer(3, GL_FLOAT, 0, myVertices.data());
		glColorPointer(4, GL_FLOAT, 0, myColors.data());
		glDrawArrays(GL_LINES, 0, myVertices.size());
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_COLOR_ARRAY);
	}
}

void vega::render::graph_view::create( const std::shared_ptr<data::volume>& v )
{
	auto pVolume = std::dynamic_pointer_cast<data::volume_graph>(myModel);
	pVolume->create(*v);

	for(auto e = pVolume->edges().begin(); e != pVolume->edges().end(); e++ )
	{
		myVertices.push_back(pVolume->get_vertex(e->source));
		myVertices.push_back(pVolume->get_vertex(e->target));
		vega::math::vector4d c = vega::math::vector4d(e->weight, e->weight, e->weight, 1.f);
		myColors.push_back(c);
		myColors.push_back(c);
	}
}

void vega::render::graph_view::mst_kruskal()
{
	auto pVolume = std::dynamic_pointer_cast<data::volume_graph>(myModel);
	typedef std::vector<data::volume_graph::edge_type> MSTContainer;
	typedef vega::graph::weighted_undirected_graph<math::vector3d> Graph;
	typedef vega::graph::disjoint_treshold_sets<Graph> DisjointSets;
	MSTContainer mst;
	
	
	std::cout << "Applying Kruskal MST... ";

	mst.resize(pVolume->get_num_vertices() - 1);
	vega::mst::kruskal2(*pVolume, mst.begin(), 0.3f);

	myVertices.clear();
	myColors.clear();

	int i = 0;
	for(auto e = mst.cbegin(); e != mst.cend(); e++ )
	{
		myVertices.push_back(pVolume->get_vertex(e->source));
		myVertices.push_back(pVolume->get_vertex(e->target));
		float k = i * 1.f / mst.size();
		vega::math::vector4d c = vega::math::vector4d(k, k, k, 1.f);
		myColors.push_back(c);
		myColors.push_back(c);
		i++;
	}

	std::cout << "Done !" << std::endl;
}


//////////////////////////////////////////////////////////////////////////
void vega::render::graph_presenter::handle_keyboard( unsigned char key, int x, int y )
{
	switch( key )
	{
	case ' ':
		{
			std::shared_ptr<graph_view> view = std::dynamic_pointer_cast<graph_view>(myView.lock());
			view->toggle_render_flag();
		}
		break;
	case 'k':
		{
			std::shared_ptr<graph_view> view = std::dynamic_pointer_cast<graph_view>(myView.lock());
			view->mst_kruskal();
		}
	}
}
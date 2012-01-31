#include <windows.h>
#include <GL/GL.h>
#include <xutility>
#include "graph_view.h"
#include "../data/volume_graph.h"
#include "../graph/graph.hpp"


void vega::render::graph_view::render() const
{
	if(myRenderFlag)
	{
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3, GL_FLOAT, 0, myVertices.data());
		glDrawArrays(GL_LINES, 0, myVertices.size());
		glDisableClientState(GL_VERTEX_ARRAY);
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
	}
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
	}
}
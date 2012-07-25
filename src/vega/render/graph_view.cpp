#include <windows.h>
#include <GL/GL.h>
#include <xutility>
#include <iostream>
#include "graph_view.h"
#include "../data/volume_graph.h"


void vega::render::graph_view::render() const
{
	if(myRenderFlag)
	{
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);
		glVertexPointer(3, GL_FLOAT, 0, myVertices.data());
		glColorPointer(4, GL_FLOAT, 0, myColors.data());
        if( myVerticesAreIndexed )
            glDrawElements(myPrimitiveIsLine ? GL_LINES : GL_POINTS, myIndices.size(), GL_UNSIGNED_INT, myIndices.data());
        else
            glDrawArrays(GL_LINES, 0, myVertices.size());
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_COLOR_ARRAY);
	}
}

bool vega::render::graph_view::create()
{
	auto pGraph = std::dynamic_pointer_cast<data::volume_graph>(myModel);

    if( pGraph == NULL )
        return false;

    myColors.clear();
    myVertices.clear();

    const math::vector3d tr(0.5f, 0.5f, 0.5f);

    myColors.insert(myColors.begin(), 2 * pGraph->get_num_edges(), math::vector4d(1.f, 1.f, 1.f, 1.f) );
	for(auto e = pGraph->edges().begin(); e != pGraph->edges().end(); e++ )
	{
        uint32 s = pGraph->get_vertex(e->source);
        uint32 t = pGraph->get_vertex(e->target);

        const data::hexagonal_prismatic_lattice::prismatic_hexagon_node& node_source = pGraph->myLattice->myLatticeCells[s];
        const data::hexagonal_prismatic_lattice::prismatic_hexagon_node& node_target = pGraph->myLattice->myLatticeCells[t];
       
		myVertices.push_back(node_source.get_vertex() * (1.f/pGraph->myLattice->myDepth) - tr);
		myVertices.push_back(node_target.get_vertex() * (1.f/pGraph->myLattice->myDepth) - tr);
	}

    return true;
}

void vega::render::graph_view::update()
{
    create();
}


//////////////////////////////////////////////////////////////////////////
void vega::render::graph_controller::handle_keyboard( unsigned char key, int x, int y )
{
	switch( key )
	{
	case ' ':
	    {
		    std::shared_ptr<graph_view> view = std::dynamic_pointer_cast<graph_view>(myView);
		    view->toggle_render_flag();
	    }
	    break;
	case 'k':
		{
			std::shared_ptr<data::volume_graph> model = std::dynamic_pointer_cast<data::volume_graph>(myModel);
			model->mst_kruskal();
		}
        break;
    case 'p':
        {
            std::shared_ptr<graph_view> view = std::dynamic_pointer_cast<graph_view>(myView);
            view->toggle_render_primitive();
        }
        break;
	}
}
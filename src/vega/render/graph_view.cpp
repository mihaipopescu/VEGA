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
        glVertexPointer(3, GL_FLOAT, 0, myVertices.data());
		glEnableClientState(GL_COLOR_ARRAY);
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

    srand(0x1234);
    std::vector<math::vector4d> color_set;
    for(size_t i=0; i<pGraph->get_num_vertices(); ++i)
    {
        color_set.push_back(math::vector4d((rand()%255)/255.f, (rand()%255)/255.f, (rand()%255)/255.f, 1.f));
    }

    const math::vector3d tr(0.5f, 0.5f, 0.5f);
    myColors.clear();
    myVertices.clear();
    for(size_t i=0; i<pGraph->get_num_vertices(); ++i)
    {
        const data::hexagonal_prismatic_lattice::prismatic_hexagon_node& node = pGraph->myLattice->myLatticeCells[i];
        myVertices.push_back(node.get_vertex() * (1.f/pGraph->myLattice->myDepth) - tr);
        myColors.push_back(color_set[pGraph->mySet[i]]);
    }

    myIndices.clear();
    for(auto e = pGraph->edges().begin(); e != pGraph->edges().end(); e++ )
    {
        myIndices.push_back(e->source);
        myIndices.push_back(e->target);
    }

    myVerticesAreIndexed = true;

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
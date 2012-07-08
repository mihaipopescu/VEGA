#include <windows.h>
#include <GL/GL.h>
#include <xutility>
#include <iostream>
#include "graph_view.h"
#include "../data/volume_graph.h"
#include "../data/graph.hpp"
#include "../algorithm/mst/kruskal.hpp"
#include "../data/disjoint_threshold_sets.hpp"

#include "../data/compact_hexagonal_lattice.h"

void vega::render::graph_view::render() const
{
	if(myRenderFlag)
	{
		glEnableClientState(GL_VERTEX_ARRAY);
		//glEnableClientState(GL_COLOR_ARRAY);
		glVertexPointer(3, GL_FLOAT, 0, myVertices.data());
		//glColorPointer(4, GL_FLOAT, 0, myColors.data());
        if( myVerticesAreIndexed )
            glDrawElements(myPrimitiveIsLine ? GL_LINES : GL_POINTS, myIndices.size(), GL_UNSIGNED_INT, myIndices.data());
        else
            glDrawArrays(GL_LINES, 0, myVertices.size());
		glDisableClientState(GL_VERTEX_ARRAY);
		//glDisableClientState(GL_COLOR_ARRAY);
	}
}

bool vega::render::graph_view::create()
{
	auto pGraph = std::dynamic_pointer_cast<data::volume_graph>(myModel);

    if( pGraph == NULL )
        return false;

	for(auto e = pGraph->edges().begin(); e != pGraph->edges().end(); e++ )
	{
        uint32 s = pGraph->get_vertex(e->source);
        uint32 t = pGraph->get_vertex(e->target);

        const data::hexagonal_prismatic_lattice::prismatic_hexagon_node& node_source = pGraph->myLattice->myLatticeCells[s];
        const data::hexagonal_prismatic_lattice::prismatic_hexagon_node& node_target = pGraph->myLattice->myLatticeCells[t];

		myVertices.push_back(node_source.get_vertex());
		myVertices.push_back(node_target.get_vertex());
	}

    return true;
}

void vega::render::graph_view::mst_kruskal()
{
	auto pGraph = std::dynamic_pointer_cast<data::volume_graph>(myModel);
	typedef std::vector<data::volume_graph::edge_type> MSTContainer;
	MSTContainer mst;
	
	std::cout << "Applying Thresholded Kruskal MST... ";

	mst.resize(pGraph->get_num_vertices() - 1);
	uint16 mst_size = vega::algorithm::mst::kruskal(*pGraph, mst.begin(), 0.3f);

    std::cout << mst.size() << " edges added !" << std::endl;

	myVertices.clear();
	myColors.clear();

    std::cout << "Creating render vertices ...";
	for(int i=0; i<mst_size; ++i)
	{
        const data::hexagonal_prismatic_lattice::prismatic_hexagon_node& node_source = pGraph->myLattice->myLatticeCells[pGraph->get_vertex(mst[i].source)];
        const data::hexagonal_prismatic_lattice::prismatic_hexagon_node& node_target = pGraph->myLattice->myLatticeCells[pGraph->get_vertex(mst[i].target)];

        myVertices.push_back(node_source.get_vertex());
        myVertices.push_back(node_target.get_vertex());
	}

	std::cout << "Done !" << std::endl;
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
			std::shared_ptr<graph_view> view = std::dynamic_pointer_cast<graph_view>(myView);
			view->mst_kruskal();
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
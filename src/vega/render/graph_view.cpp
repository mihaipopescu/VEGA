#include <windows.h>
#include <GL/GL.h>
#include <xutility>
#include <iostream>
#include "graph_view.h"
#include "../data/volume_graph.h"
#include "../graph/graph.hpp"
#include "../algorithm/mst/kruskal.hpp"
#include "../graph/disjoint_threshold_sets.hpp"

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

void vega::render::graph_view::create( vega::uint8 width, vega::uint8 height, vega::uint8 depth )
{
    vega::data::compact_hexagonal_lattice chl;
    chl.create(width, height, depth, 1.0f);
    
	myVertices.insert(myVertices.begin(), chl.myLattice.begin(), chl.myLattice.end());
    myIndices.insert(myIndices.begin(), chl.myLines.begin(), chl.myLines.end());

    myVerticesAreIndexed = true;
}

void vega::render::graph_view::create( const std::shared_ptr<data::volume>& v )
{
	auto pVolume = std::dynamic_pointer_cast<data::volume_graph>(myModel);
	pVolume->create(*v);

	for(auto e = pVolume->edges().begin(); e != pVolume->edges().end(); e++ )
	{
		myVertices.push_back(pVolume->get_vertex(e->source));
		myVertices.push_back(pVolume->get_vertex(e->target));
        //float k = 1.f; //e->weight;
        //vega::math::vector4d c = vega::math::vector4d(k, k, k, 1.f);
		//myColors.push_back(c);
		//myColors.push_back(c);
	}
}

void vega::render::graph_view::mst_kruskal()
{
	auto pVolume = std::dynamic_pointer_cast<data::volume_graph>(myModel);
	typedef std::vector<data::volume_graph::edge_type> MSTContainer;
	typedef vega::graph::weighted_undirected_graph<math::vector3d> Graph;
	typedef vega::graph::disjoint_treshold_sets<Graph> DisjointSets;
	MSTContainer mst;
	
	std::cout << "Applying Thresholded Kruskal MST... ";

	mst.resize(pVolume->get_num_vertices() - 1);
	uint16 mst_size = vega::algorithm::mst::kruskal(*pVolume, mst.begin(), 0.3f);

    std::cout << mst.size() << " edges added !" << std::endl;

	myVertices.clear();
	myColors.clear();

    std::cout << "Creating render vertices ...";
	for(int i=0; i<mst_size; ++i)
	{
        Graph::edge_type e = mst[i];
        myVertices.push_back(pVolume->get_vertex(e.source));
        myVertices.push_back(pVolume->get_vertex(e.target));
		//float k = 1.f;
		//vega::math::vector4d c = vega::math::vector4d(k, k, k, 1.f);
		//myColors.push_back(c);
		//myColors.push_back(c);
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
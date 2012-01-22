#include "volume_graph.h"
#include "../graph/hexagonal_prismatic_lattice.h"
#include <iostream>


using namespace vega;
using namespace vega::math;

bool vega::data::volume_graph::create( const volume& v )
{
    volume::create(v);

	graph::hexagonal_prismatic_lattice * hpl = new graph::hexagonal_prismatic_lattice(v);

	std::cout << "Generating volume graph ..." << std::endl;


	std::cout << "Cleaning up ..." << std::endl;
	delete hpl;

    return true;
}

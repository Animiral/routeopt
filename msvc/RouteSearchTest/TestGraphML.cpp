#include <fstream>
#include "catch.hpp"
#include "Graph.h"

//typedef boost::graph::property<boost::graph::vertex_distance_t, float, 
//        boost::graph::property<boost::graph::vertex_name_t, std::string> > VertexProperty;
//typedef boost::graph::property<boost::graph::edge_weight_t, float> EdgeProperty;

//typedef boost::adjacency_list<> AdjList;

TEST_CASE("Loading a GraphML graph")
{
	std::ifstream ifstream("basic.graphml");
	std::string graphText;
	if(std::getline(ifstream, graphText, {})) {
		AirGraph graph{AirGraph::FromGraphML(graphText)};
		REQUIRE(6 == graph.Waypoints().size());
	}
}

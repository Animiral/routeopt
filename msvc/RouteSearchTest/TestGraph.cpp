// Tests for basic functions of our Graph class

#include "catch.hpp"
#include "Graph.h"

TEST_CASE( "Graph construction" )
{
	Graph graph;
	Node a{10, 10, 100};
	Node b{20, 20, 110};
	graph.AddNode(a);
	graph.AddNode(b);
	graph.AddNode({30, 10, 100});
	graph.AddEdge({a, b});

	auto const& nodes = graph.Nodes();
	REQUIRE(3 == nodes.size());
	REQUIRE(a.latitude == nodes[0].latitude);
}

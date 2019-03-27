// Tests for basic functions of our Graph class

#define CATCH_CONFIG_MAIN // TODO: move this shit
#include "catch.hpp"
#include "Graph.h"

TEST_CASE( "Graph construction" )
{
	Graph graph;
	Node const& a = graph.AddNode({10, 10, 100});
	graph.AddNode({20, 20, 110});
	graph.AddNode({30, 10, 100});
	graph.AddEdge({a, b});

	auto const& nodes = graph.Nodes();
	REQUIRE(3 == nodes.size());
	REQUIRE(a.latitude == nodes[0].latitude);
}
//
//TEST_CASE( "Graph construction" )
//{
//	Graph graph;
//
//	REQUIRE(true);
//}


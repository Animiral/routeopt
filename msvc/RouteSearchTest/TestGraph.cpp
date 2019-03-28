// Tests for basic functions of our Graph class

#include "catch.hpp"
#include "Graph.h"

TEST_CASE("Graph construction")
{
	Graph graph;
	Node a{10, 10, 100};
	Node b{20, 20, 110};
	graph.AddNode(a);
	graph.AddNode(b);

	SECTION("The Graph stores Nodes")
	{
		auto const& nodes = graph.Nodes();
		REQUIRE(2 == nodes.size());
		REQUIRE(a == nodes[0]);
		REQUIRE(b == nodes[1]);
	}

	SECTION("The Graph stores Edges")
	{
		graph.AddEdge({a, b});
		REQUIRE(std::vector<Node>{b} == graph.Neighbors(a));
	}

	SECTION("Throw on Neighbors() of nonexistant Node")
	{
		REQUIRE_THROWS(graph.Neighbors({0,0,0}));
	}

	SECTION("Throw on AddEdge() from nonexistant Node")
	{
		REQUIRE_THROWS(graph.AddEdge({{0,0,0}, b}));
	}

	SECTION("Throw on AddEdge() to nonexistant Node")
	{
		REQUIRE_THROWS(graph.AddEdge({a, {0,0,0}}));
	}
}

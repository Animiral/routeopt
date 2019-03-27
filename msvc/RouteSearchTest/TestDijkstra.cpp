// Tests for our search algorithm

#include "catch.hpp"
#include "Dijkstra.h"
#include "Checks.h"

TEST_CASE( "Trivial search" )
{
	Graph graph;
	Node& a = graph.AddNode({10, 10, 100});
	Node& b = graph.AddNode({20, 20, 110});
	graph.AddEdge({a, b});

	Dijkstra dijkstra{graph};
	dijkstra.run(a, b);
	Path const& actual = dijkstra.result();
	Path expected{a, b};

	REQUIRE(IsPathEqual(actual, expected));
}

TEST_CASE( "Shorter Path" )
{
	// In this test case, despite the path via b traversing less nodes,
	// the path via c and d is cheaper.
	//
	//   --- b ---
	//  /         \
	// a - c - d - e
	Graph graph;
	Node& a = graph.AddNode({0, 0, 100});
	Node& b = graph.AddNode({100, 15, 100});
	Node& c = graph.AddNode({0, 10, 100});
	Node& d = graph.AddNode({0, 20, 100});
	Node& e = graph.AddNode({0, 30, 100});
	graph.AddEdge({a, b});
	graph.AddEdge({b, e});
	graph.AddEdge({a, c});
	graph.AddEdge({c, d});
	graph.AddEdge({d, e});

	Dijkstra dijkstra{graph};
	dijkstra.run(a, e);
	Path const& actual = dijkstra.result();
	Path expected{a, c, d, e};
	
	REQUIRE(IsPathEqual(actual, expected));
}

TEST_CASE("Unsuccessful Search")
{
	Graph graph;
	Node& a = graph.AddNode({10, 10, 100});
	Node& b = graph.AddNode({20, 20, 110});

	Dijkstra dijkstra{graph};
	dijkstra.run(a, b);
	Path const& result = dijkstra.result();

	REQUIRE(result.empty());
}

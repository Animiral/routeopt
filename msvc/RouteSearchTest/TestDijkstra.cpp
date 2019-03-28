// Tests for our search algorithm

#include "catch.hpp"
#include "Dijkstra.h"
#include "Checks.h"

TEST_CASE( "Trivial search" )
{
	Graph graph;
	Node a{10, 10, 100};
	Node b{20, 20, 110};
	graph.AddNode(a);
	graph.AddNode(b);
	graph.AddEdge({a, b});

	Dijkstra dijkstra{graph};
	dijkstra.run(a, b);
	Path const& actual = dijkstra.result();
	Path expected{a, b};

	REQUIRE(actual == expected);
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
	Node a{0, 0, 100};
	Node b{100, 15, 100};
	Node c{0, 10, 100};
	Node d{0, 20, 100};
	Node e{0, 30, 100};
	graph.AddNode(a);
	graph.AddNode(b);
	graph.AddNode(c);
	graph.AddNode(d);
	graph.AddNode(e);
	graph.AddEdge({a, b});
	graph.AddEdge({b, e});
	graph.AddEdge({a, c});
	graph.AddEdge({c, d});
	graph.AddEdge({d, e});

	Dijkstra dijkstra{graph};
	dijkstra.run(a, e);
	Path const& actual = dijkstra.result();
	Path expected{a, c, d, e};
	
	REQUIRE(actual == expected);
}

TEST_CASE("Unsuccessful Search")
{
	Graph graph;
	Node a{10, 10, 100};
	Node b{20, 20, 110};
	graph.AddNode(a);
	graph.AddNode(b);

	Dijkstra dijkstra{graph};
	dijkstra.run(a, b);
	Path const& result = dijkstra.result();

	REQUIRE(result.empty());
}

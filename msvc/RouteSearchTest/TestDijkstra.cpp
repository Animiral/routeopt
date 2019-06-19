// Tests for our search algorithm

#include "catch.hpp"
#include "Dijkstra.h"
#include "Checks.h"

namespace
{

Cost cost; // dependency for all Dijkstra objects

}

TEST_CASE("Trivial search")
{
	AirGraph graph;
	Waypoint a{10, 10, 100};
	Waypoint b{20, 20, 110};
	graph.AddWaypoint(a);
	graph.AddWaypoint(b);
	graph.AddAirway({a, b});

	Dijkstra dijkstra{graph, cost};
	dijkstra.run(a, b);
	Path const& actual = dijkstra.result();
	Path expected{a, b};

	REQUIRE(actual == expected);
}

TEST_CASE("Shorter Path")
{
	// In this test case, despite the path via b traversing less nodes,
	// the path via c and d is cheaper.
	//
	//   --- b ---
	//  /         \
	// a - c - d - e
	AirGraph graph;
	Waypoint a{0, 0, 100};
	Waypoint b{100, 15, 100};
	Waypoint c{0, 10, 100};
	Waypoint d{0, 20, 100};
	Waypoint e{0, 30, 100};
	graph.AddWaypoint(a);
	graph.AddWaypoint(b);
	graph.AddWaypoint(c);
	graph.AddWaypoint(d);
	graph.AddWaypoint(e);
	graph.AddAirway({a, b});
	graph.AddAirway({b, e});
	graph.AddAirway({a, c});
	graph.AddAirway({c, d});
	graph.AddAirway({d, e});

	Dijkstra dijkstra{graph, cost};
	dijkstra.run(a, e);
	Path const& actual = dijkstra.result();
	Path expected{a, c, d, e};
	
	REQUIRE(actual == expected);
}

TEST_CASE("Unsuccessful Search")
{
	AirGraph graph;
	Waypoint a{10, 10, 100};
	Waypoint b{20, 20, 110};
	graph.AddWaypoint(a);
	graph.AddWaypoint(b);

	Dijkstra dijkstra{graph, cost};
	dijkstra.run(a, b);
	Path const& result = dijkstra.result();

	REQUIRE(result.empty());
}

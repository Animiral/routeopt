// Tests for basic functions of our AirGraph class

#include "catch.hpp"
#include "Graph.h"

TEST_CASE("AirGraph construction")
{
	AirGraph graph;
	Waypoint a{10, 10, 100};
	Waypoint b{20, 20, 110};
	graph.AddWaypoint(a);
	graph.AddWaypoint(b);

	SECTION("The AirGraph stores Waypoints")
	{
		auto const& nodes = graph.Waypoints();
		REQUIRE(2 == nodes.size());
		REQUIRE(a == nodes[0]);
		REQUIRE(b == nodes[1]);
	}

	SECTION("The AirGraph stores Edges")
	{
		graph.AddAirway({a, b});
		REQUIRE(std::vector<Waypoint>{b} == graph.Neighbors(a));
	}

	SECTION("Throw on Neighbors() of nonexistant Waypoint")
	{
		REQUIRE_THROWS(graph.Neighbors({0,0,0}));
	}

	SECTION("Throw on AddAirway() from nonexistant Waypoint")
	{
		REQUIRE_THROWS(graph.AddAirway({{0,0,0}, b}));
	}

	SECTION("Throw on AddAirway() to nonexistant Waypoint")
	{
		REQUIRE_THROWS(graph.AddAirway({a, {0,0,0}}));
	}
}

// Tests for basic functions of our AirGraph class

#include "catch.hpp"
#include "Graph.h"

TEST_CASE("AirGraph construction")
{
	AirGraph graph;
	Waypoint wp_a{10, 10, 100}; /* lat, lng, fl */
	Waypoint wp_b{20, 20, 110};
	Airway awy_ab{30000, 14.1f, 0.1f, 50.0f}; /* gm, dist, time, fuel */

	AirGraph::NodeId a_id = graph.AddNode(wp_a);
	AirGraph::NodeId b_id = graph.AddNode(wp_b);
	AirGraph::EdgeId ab_id = graph.AddEdge(a_id, b_id, awy_ab);

	SECTION("Throw on AddEdge() from nonexistant Node")
	{
		REQUIRE_THROWS(graph.AddEdge((AirGraph::NodeId)100, b_id, {0,0,0,0}));
	}

	SECTION("Throw on AddEdge() to nonexistant Node")
	{
		REQUIRE_THROWS(graph.AddEdge(b_id, (AirGraph::NodeId)100, {0,0,0,0}));
	}

	SECTION("retrieve Node by id")
	{
		auto const& node = graph.GetNode(a_id);
		CHECK(a_id == node.id);
		CHECK(wp_a == node.waypoint);
	}

	SECTION("Throw on retrieve Node by invalid id")
	{
		REQUIRE_THROWS(graph.GetNode((AirGraph::NodeId)100));
	}

	SECTION("retrieve list of Nodes from the AirGraph")
	{
		auto const& nodes = graph.GetNodes();
		REQUIRE(2 == nodes.size());
		CHECK(a_id == nodes[0].id);
		CHECK(wp_a == nodes[0].waypoint);
		CHECK(b_id == nodes[1].id);
		CHECK(wp_b == nodes[1].waypoint);
	}

	SECTION("retrieve Edge by id")
	{
		auto const& edge = graph.GetEdge(ab_id);
		CHECK(ab_id == edge.id);
		CHECK(a_id == edge.from);
		CHECK(b_id == edge.to);
		CHECK(awy_ab == edge.airway);
	}

	SECTION("Throw on retrieve Edge by invalid id")
	{
		REQUIRE_THROWS(graph.GetEdge((AirGraph::EdgeId)100));
	}

	SECTION("retrieve list of Edges from the AirGraph")
	{
		auto const& edges = graph.GetEdges();
		REQUIRE(1 == edges.size());
		CHECK(ab_id == edges[0].id);
	}

	SECTION("retrieve successors of nodes")
	{
		auto const& out_edges = graph.GetNode(a_id).out_edges;
		REQUIRE(1 == out_edges.size());
		CHECK(ab_id == out_edges[0]);
	}
}

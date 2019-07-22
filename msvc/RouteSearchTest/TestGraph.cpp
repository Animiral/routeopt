// Tests for basic functions of our AirGraph class

#include <algorithm>
#include "catch.hpp"
#include "Graph.h"

TEST_CASE("AirGraph Structure")
{
	AirGraph graph;
	Waypoint wp_a{"a", 10, 10, 100}; /* name, lat, lng, fl */
	Waypoint wp_b{"b", 20, 20, 110};
	Airway awy_ab{"ab", 30000, 14.1f, 0.1f, 50.0f}; /* name, gm, dist, time, fuel */

	AirGraph::NodeId a_id = graph.AddNode(wp_a);
	AirGraph::NodeId b_id = graph.AddNode(wp_b);
	AirGraph::EdgeId ab_id = graph.AddEdge(a_id, b_id, awy_ab);

	SECTION("Throw on AddEdge() from nonexistant Node")
	{
		REQUIRE_THROWS(graph.AddEdge((AirGraph::NodeId)100, b_id, {"e",0,0,0,0}));
	}

	SECTION("Throw on AddEdge() to nonexistant Node")
	{
		REQUIRE_THROWS(graph.AddEdge(b_id, (AirGraph::NodeId)100, {"e",0,0,0,0}));
	}

	SECTION("retrieve Node by id")
	{
		auto const& node = graph.GetNode(a_id);
		CHECK(a_id == node.id);
		CHECK(wp_a == node.waypoint);
	}

	SECTION("retrieve Node by name")
	{
		auto const* node = graph.FindNodeByName("a");
		CHECK(a_id == node->id);
		CHECK(wp_a == node->waypoint);
	}

	SECTION("Node does not exist by name")
	{
		auto const* node = graph.FindNodeByName("ab");
		CHECK(nullptr == node);
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

	SECTION("retrieve Edge by name")
	{
		auto const& edge = graph.FindEdgeByName("ab");
		CHECK(ab_id == edge->id);
		CHECK(a_id == edge->from);
		CHECK(b_id == edge->to);
		CHECK(awy_ab == edge->airway);
	}

	SECTION("Edge does not exist by name")
	{
		auto const* edge = graph.FindEdgeByName("a");
		CHECK(nullptr == edge);
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

TEST_CASE("Disable Node of AirGraph")
{
	AirGraph graph;

	AirGraph::NodeId a_id = graph.AddNode({"a", 10, 10, 100});
	graph.SetNodeDisabled(a_id, true);
	CHECK(graph.GetNode(a_id).disabled);
}


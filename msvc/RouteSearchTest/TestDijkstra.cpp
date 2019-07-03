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
	AirGraph::NodeId a_id = graph.AddNode(a);
	AirGraph::NodeId b_id = graph.AddNode(b);
	AirGraph::EdgeId ab_id = graph.AddEdge(a_id, b_id, {20000, 100.f, .5f, 100.f});

	Dijkstra dijkstra{graph, cost};
	dijkstra.run(a_id, b_id);
	Path const& actual = dijkstra.result();
	std::vector<AirGraph::NodeId> expected_nodes{a_id, b_id};
	std::vector<AirGraph::EdgeId> expected_edges{ab_id};

	CHECK(actual.nodes == expected_nodes);
	CHECK(actual.edges == expected_edges);
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
	AirGraph::NodeId a_id = graph.AddNode(a);
	AirGraph::NodeId b_id = graph.AddNode(b);
	AirGraph::NodeId c_id = graph.AddNode(c);
	AirGraph::NodeId d_id = graph.AddNode(d);
	AirGraph::NodeId e_id = graph.AddNode(e);
	graph.AddEdge(a_id, b_id, {20000, 150.f, .5f, 100.f});
	graph.AddEdge(b_id, e_id, {20000, 150.f, .5f, 100.f});
	AirGraph::EdgeId ac_id = graph.AddEdge(a_id, c_id, {20000, 10.f, .5f, 100.f});
	AirGraph::EdgeId cd_id = graph.AddEdge(c_id, d_id, {20000, 10.f, .5f, 100.f});
	AirGraph::EdgeId de_id = graph.AddEdge(d_id, e_id, {20000, 200.f, .5f, 100.f});

	Dijkstra dijkstra{graph, cost};
	dijkstra.run(a_id, e_id);
	Path const& actual = dijkstra.result();
	std::vector<AirGraph::NodeId> expected_nodes{a_id, c_id, d_id, e_id};
	std::vector<AirGraph::EdgeId> expected_edges{ac_id, cd_id, de_id};

	CHECK(actual.nodes == expected_nodes);
	CHECK(actual.edges == expected_edges);
}

TEST_CASE("Unsuccessful Search")
{
	AirGraph graph;
	Waypoint a{10, 10, 100};
	Waypoint b{20, 20, 110};
	AirGraph::NodeId a_id = graph.AddNode(a);
	AirGraph::NodeId b_id = graph.AddNode(b);

	Dijkstra dijkstra{graph, cost};
	dijkstra.run(a_id, b_id);
	Path const& result = dijkstra.result();

	CHECK(result.nodes.empty());
}

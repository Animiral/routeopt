// Tests for our search algorithm

#include "catch.hpp"
#include "Dijkstra.h"
#include "Graph.h"
#include "Scenario.h"

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
	Path const& actual = dijkstra.Run(a_id, b_id);
	std::vector<AirGraph::NodeId> expected_nodes{a_id, b_id};
	std::vector<AirGraph::EdgeId> expected_edges{ab_id};

	CHECK(actual.nodes == expected_nodes);
	CHECK(actual.edges == expected_edges);
	CHECK(actual.cost == 100.f);
}

TEST_CASE("Shorter Path")
{
	// In this test case, despite the path via b traversing less nodes,
	// the path via c and d is cheaper.
	Scenario1 s;

	Dijkstra dijkstra{s.graph, cost};
	Path const& actual = dijkstra.Run(s.a_id, s.e_id);
	std::vector<AirGraph::NodeId> expected_nodes{s.a_id, s.c_id, s.d_id, s.e_id};
	std::vector<AirGraph::EdgeId> expected_edges{s.ac_id, s.cd_id, s.de_id};

	CHECK(actual.nodes == expected_nodes);
	CHECK(actual.edges == expected_edges);
	CHECK(actual.cost == Approx(220.f));
}

TEST_CASE("Unsuccessful Search")
{
	AirGraph graph;
	Waypoint a{10, 10, 100};
	Waypoint b{20, 20, 110};
	AirGraph::NodeId a_id = graph.AddNode(a);
	AirGraph::NodeId b_id = graph.AddNode(b);

	Dijkstra dijkstra{graph, cost};
	Path const& result = dijkstra.Run(a_id, b_id);

	CHECK(result.nodes.empty());
}

TEST_CASE("Search around Disabled Node")
{
	// In this test case, node c is disabled.
	// The correct solution therefore must use node b.
	Scenario1 s;

	s.graph.SetNodeDisabled(s.c_id, true);
	Dijkstra dijkstra{s.graph, cost};
	Path const& actual = dijkstra.Run(s.a_id, s.e_id);
	std::vector<AirGraph::NodeId> expected_nodes{s.a_id, s.b_id, s.e_id};
	std::vector<AirGraph::EdgeId> expected_edges{s.ab_id, s.be_id};

	CHECK(actual.nodes == expected_nodes);
	CHECK(actual.edges == expected_edges);
	CHECK(actual.cost == Approx(300.f));
}

TEST_CASE("Counter With No Restrictions")
{
	// We Run a small search and check whether it produces the
	// expected number of performance events.
	Scenario1 s;
	Counter counter;

	Dijkstra dijkstra{s.graph, cost};
	dijkstra.SetCounter(counter);
	dijkstra.Run(s.a_id, s.e_id);

	CHECK(counter.CountNodeVisited() == 5); // every node was visited
	CHECK(counter.CountEdgeVisited() == 5); // every edge was visited
	CHECK(counter.CountPathFound() == 1); // one candidate solution
	CHECK(counter.CountRestrictionViolated() == 0); // no restrictions
}

TEST_CASE("Bug continue out_edges")
{
	// Tests that a bug is fixed in which not all outgoing edges
	// of a node were explored if one of them lead to a closed node.
	Scenario2 s;
	Counter counter;

	Dijkstra dijkstra{s.graph, cost};
	dijkstra.SetCounter(counter);
	dijkstra.Run(s.a_id, s.f_id);

	CHECK(counter.CountNodeVisited() == 6); // every node was visited
	CHECK(counter.CountEdgeVisited() == 5); // every edge was visited except 3->2 and 5->4
	CHECK(counter.CountPathFound() == 1); // one candidate solution
	CHECK(counter.CountRestrictionViolated() == 0); // no restrictions
}

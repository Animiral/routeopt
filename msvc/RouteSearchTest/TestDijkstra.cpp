// Tests for our search algorithm

#include "catch.hpp"
#include "Dijkstra.h"
#include "Graph.h"
#include "Checks.h"

namespace
{

Cost cost; // dependency for all Dijkstra objects

/**
 * A small test graph with 5 nodes.
 *
 *   --- b ---
 *  /         \
 * a - c - d - e
 */
struct Scenario1
{
	explicit Scenario1()
		:
		graph(),
		a_id(graph.AddNode({0, 0, 100})   ),
		b_id(graph.AddNode({100, 15, 100})),
		c_id(graph.AddNode({0, 10, 100})  ),
		d_id(graph.AddNode({0, 20, 100})  ),
		e_id(graph.AddNode({0, 30, 100})  )
	{
		graph.AddEdge(a_id, b_id, {20000, 150.f, .5f, 100.f});
		graph.AddEdge(b_id, e_id, {20000, 150.f, .5f, 100.f});
		ac_id = graph.AddEdge(a_id, c_id, {20000, 10.f, .5f, 100.f});
		cd_id = graph.AddEdge(c_id, d_id, {20000, 10.f, .5f, 100.f});
		de_id = graph.AddEdge(d_id, e_id, {20000, 200.f, .5f, 100.f});
	}

	AirGraph graph;
	AirGraph::NodeId a_id, b_id, c_id, d_id, e_id;
	AirGraph::EdgeId ac_id, cd_id, de_id;
};

/**
 * Another test graph with 6 nodes.
 *
 *  -->1-->3--->5-\
 * 0        \      -->4
 *  ----------->2-/
 */
struct Scenario2
{
	explicit Scenario2()
		:
		graph(),
		a_id(graph.AddNode({0, 0, 100})   ),
		b_id(graph.AddNode({100, -50, 100})),
		c_id(graph.AddNode({300, 50, 100})  ),
		d_id(graph.AddNode({200, -50, 100})  ),
		e_id(graph.AddNode({300, -50, 100})  ),
		f_id(graph.AddNode({400, 0, 100})  )
	{
		graph.AddEdge(a_id, b_id, {20000, 10.f, .5f, 100.f});
		graph.AddEdge(a_id, c_id, {20000, 10.f, .5f, 100.f});
		graph.AddEdge(b_id, d_id, {20000, 10.f, .5f, 100.f});
		graph.AddEdge(d_id, c_id, {20000, 5.f, .5f, 100.f});
		graph.AddEdge(c_id, e_id, {20000, 10.f, .5f, 100.f});
		graph.AddEdge(d_id, f_id, {20000, 10.f, .5f, 100.f});
		graph.AddEdge(f_id, e_id, {20000, 10.f, .5f, 100.f});
	}

	AirGraph graph;
	AirGraph::NodeId a_id, b_id, c_id, d_id, e_id, f_id;
};

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
	dijkstra.Run(a_id, b_id);
	Path const& actual = dijkstra.Result();
	std::vector<AirGraph::NodeId> expected_nodes{a_id, b_id};
	std::vector<AirGraph::EdgeId> expected_edges{ab_id};

	CHECK(actual.nodes == expected_nodes);
	CHECK(actual.edges == expected_edges);
}

TEST_CASE("Shorter Path")
{
	// In this test case, despite the path via b traversing less nodes,
	// the path via c and d is cheaper.
	Scenario1 s;

	Dijkstra dijkstra{s.graph, cost};
	dijkstra.Run(s.a_id, s.e_id);
	Path const& actual = dijkstra.Result();
	std::vector<AirGraph::NodeId> expected_nodes{s.a_id, s.c_id, s.d_id, s.e_id};
	std::vector<AirGraph::EdgeId> expected_edges{s.ac_id, s.cd_id, s.de_id};

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
	dijkstra.Run(a_id, b_id);
	Path const& result = dijkstra.Result();

	CHECK(result.nodes.empty());
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

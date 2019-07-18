// Tests for airspace Restrictions

#include <algorithm>
#include "catch.hpp"
#include "Restriction.h"
#include "Graph.h"
#include "Dijkstra.h"
#include "Scenario.h"

TEST_CASE("Restriction Violated")
{
	std::vector<AirGraph::NodeId> nodes{0, 3, 5, 6, 7};
	std::vector<AirGraph::EdgeId> edges{1, 2, 4, 5};
	Path path{nodes, edges};

	SECTION("Restriction which is violated on test path")
	{
		Restriction restriction1{3, 7};
		CHECK(restriction1.CheckViolated(path));
	}

	SECTION("Restriction which is not violated on test path")
	{
		Restriction restriction2{1, 5};
		CHECK(!restriction2.CheckViolated(path));
	}
}

TEST_CASE("Prevent Restriction")
{
	Scenario1 s;

	Restriction restriction{s.a_id, s.c_id};
	auto graphs = restriction.Prevent(s.graph);

	const auto node_equal = [](const AirGraph::Node& n, AirGraph::NodeId nid) { return n.id == nid; };
	using V = std::vector<AirGraph::NodeId>;

	const auto& nodes1 = graphs.first.GetNodes();
	V ex_nodes1{s.b_id, s.c_id, s.d_id, s.e_id};
	CHECK(std::equal(nodes1.begin(), nodes1.end(),
	                 ex_nodes1.begin(), ex_nodes1.end(),
	                 node_equal));
	
	const auto& nodes2 = graphs.second.GetNodes();
	V ex_nodes2{s.a_id, s.b_id, s.d_id, s.e_id};
	CHECK(std::equal(nodes2.begin(), nodes2.end(),
	                 ex_nodes2.begin(), ex_nodes2.end(),
	                 node_equal));

	const auto edge_equal = [](const AirGraph::Edge& e, AirGraph::EdgeId eid) { return e.id == eid; };
	using E = std::vector<AirGraph::EdgeId>;

	const auto& edges1 = graphs.first.GetEdges();
	E ex_edges1{s.be_id, s.cd_id, s.de_id};
	CHECK(std::equal(edges1.begin(), edges1.end(),
	                 ex_edges1.begin(), ex_edges1.end(),
	                 edge_equal));
	
	const auto& edges2 = graphs.second.GetEdges();
	E ex_edges2{s.ab_id, s.be_id, s.de_id};
	CHECK(std::equal(edges2.begin(), edges2.end(),
	                 ex_edges2.begin(), ex_edges2.end(),
	                 edge_equal));
}

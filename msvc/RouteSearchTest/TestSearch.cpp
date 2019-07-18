/**
 * Tests for our search algorithm that obeys restrictions.
 */

#include "catch.hpp"
#include "Scenario.h"
#include "Search.h"
#include "Graph.h"
#include "Dijkstra.h"
#include "Restriction.h"

TEST_CASE("Search with Restrictions")
{
	Scenario3 s;

	Cost cost;
	Dijkstra dijkstra{s.graph, cost};
	std::vector<Restriction> restrictions{s.restriction};
	Search search{s.graph, dijkstra, restrictions};

	Path const& actual = search.Run(s.a_id, s.d_id);
	std::vector<AirGraph::NodeId> expected_nodes{s.a_id, s.c_id, s.d_id};
	std::vector<AirGraph::EdgeId> expected_edges{s.ac_id, s.cd_id};

	CHECK(actual.nodes == expected_nodes);
	CHECK(actual.edges == expected_edges);
}

TEST_CASE("Counter With Restrictions")
{
	Scenario3 s;
	Counter counter;

	Cost cost;
	Dijkstra dijkstra{s.graph, cost};
	dijkstra.SetCounter(counter);
	std::vector<Restriction> restrictions{s.restriction};
	Search search{s.graph, dijkstra, restrictions};
	search.SetCounter(counter);
	
	search.Run(s.a_id, s.d_id);

	CHECK(counter.CountNodeVisited() == 4+3+3); // from 3 search runs
	CHECK(counter.CountEdgeVisited() == 5+2+2); // from 3 search runs
	CHECK(counter.CountPathFound() == 3); // all searches produce candidates
	CHECK(counter.CountRestrictionViolated() == 1); // one restriction
}

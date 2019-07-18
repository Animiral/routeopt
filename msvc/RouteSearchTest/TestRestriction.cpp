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

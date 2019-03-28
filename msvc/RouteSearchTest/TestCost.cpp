// Tests for the edge cost implementation.
// The cost is (will be) based on a simplified aircraft performance data implementation.
// In a real scenario, the cost depends on many factors from ac data to weather forecasts.

#include "catch.hpp"
#include "Graph.h"
#include "Cost.h"

TEST_CASE("Successful cost calculation")
{
	Cost cost;
	Edge edge{{0, 0, 100}, {0, 10, 100}};
	// TODO: implement cost
	REQUIRE((cost.Calculate(edge) || "TODO: implement this test case"));
}

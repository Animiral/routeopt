// Tests for the edge cost implementation.
// The cost is (will be) based on a simplified aircraft performance data implementation.
// In a real scenario, the cost depends on many factors from ac data to weather forecasts.

#include "catch.hpp"
#include "Graph.h"
#include "Cost.h"

TEST_CASE("Successful cost calculation")
{
	Cost cost;
	Airway airway{"awy", 30000, 14.1f, 0.1f, 50.0f};
	// TODO: implement cost
	REQUIRE((cost.Calculate(airway) || "TODO: implement this test case"));
}

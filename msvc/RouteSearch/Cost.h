#pragma once

#include "Graph.h"

/**
 * The comparable type which we use to represent the definite cost of something.
 */
using CostValue = float;

/**
 * This is a simplified implementation of the cost function for travel
 * through the airspace.
 * The cost is (will be) based on a simplified aircraft performance data implementation.
 * In a real scenario, the cost depends on many factors from ac data to weather forecasts.
 */
class Cost
{

public:

	/**
	 * Return the cost of flying along the specified edge.
	 */
	CostValue Calculate(Edge edge) const;

};

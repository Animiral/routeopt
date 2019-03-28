#pragma once

#include "Graph.h"

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
	float Calculate(Edge edge) const;

};

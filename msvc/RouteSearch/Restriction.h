#pragma once

#include "Graph.h"
#include "Path.h"

/**
 * Our dumb model for airspace restrictions.
 * The formally difficult kind of restrictions that we are examining consist
 * of forbidden pairs of graph waypoints or nodes.
 *
 * In our solution implementation, we are only interested in two aspects of
 * restrictions:
 * 1. Check whether a proposed route solution violates the restriction.
 * 2. Modify the airspace graph such that another violation becomes impossible.
 *
 * The modification/resolution logic is implemented in the search algorithm.
 */
struct Restriction
{
	AirGraph::NodeId a, b;

	/**
	 * Return true iff the proposed solution path violates this restriction.
	 */
	bool CheckViolated(const Path& path) const noexcept;
};


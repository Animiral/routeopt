#pragma once

#include "Graph.h"

/**
 * Our model for airspace restrictions.
 * The formally difficult kind of restrictions that we are examining consist
 * of forbidden pairs of graph waypoints or nodes.
 *
 * In our solution implementation, we are only interested in two aspects of
 * restrictions:
 * 1. Check whether a proposed route solution violates the restriction.
 * 2. Modify the airspace graph such that another violation becomes impossible.
 */
class Restriction
{

public:

	/**
	 * Construct a restriction from a forbidden pair.
	 */
	explicit Restriction(AirGraph::NodeId a, AirGraph::NodeId b) noexcept;

	/**
	 * Return true iff the proposed solution path violates this restriction.
	 */
	bool CheckViolated(const Path& path) const noexcept;

	/**
	 * Deduce two variations of the Graph. Each of the variations can be solved
	 * without this restriction. We must then choose the better of the two.
	 */
	std::pair<AirGraph, AirGraph> Prevent(AirGraph graph);

private:

	AirGraph::NodeId m_a;
	AirGraph::NodeId m_b;

};


#pragma once

/**
 * The Counter counts events related to route optimization performance.
 * These are the metrics that we are trying to optimize.
 */
class Counter
{

public:

	Counter();

	/**
	 * Signal that a waypoint has been visited in the graph search.
	 */
	void NodeVisited();

	/**
	 * Signal that an airway (segment) has been visited in the graph search.
	 */
	void EdgeVisited();

	/**
	 * Signal that a candidate solution has been found in the graph search.
	 */
	void PathFound();

	/**
	 * Signal that we did a lookup on aircraft performance data.
	 * Since we do not model these lookups for now, it is disabled.
	 */
	//void ACperfLookup();

	/**
	 * Signal that a candidate path has been found to violate some restriction.
	 */
	void RestrictionViolated();

	/**
	 * Return the stored number of waypoints visited.
	 */
	long CountNodeVisited();

	/**
	 * Return the stored number of airways visited.
	 */
	long CountEdgeVisited();

	/**
	 * Return the stored number of solutions found.
	 */
	long CountPathFound();

	/**
	 * Return the stored number of restrictions violated.
	 */
	long CountRestrictionViolated();

private:

	long m_node_visited; //!< internal counter for node_visited
	long m_edge_visited; //!< internal counter for edge_visited
	long m_path_found; //!< internal counter for path_found
	long m_restriction_violated; //!< internal counter for restriction_violated

};


#pragma once

#include <vector>
#include "Graph.h"
#include "Dijkstra.h"
#include "Restriction.h"

/**
 * Our implementation of a route search algorithm.
 * It utilizes Dijkstra's algorithm for shortest paths with a modification
 * to support restrictions.
 */
class Search
{

public:

	explicit Search(AirGraph& graph, const Dijkstra& dijkstra, std::vector<Restriction> restrictions) noexcept;

	/**
	 * Configure the search to count performance events.
	 */
	void SetCounter(Counter& counter);

	/**
	 * Return the cheapest path from the start node to the goal node.
	 * The resultant path observes all restrictions.
	 * If no search was executed or the search was not successful, return an empty path.
	 * After the run, this Search object is invalidated and not to be reused.
	 */
	Path Run(AirGraph::NodeId start, AirGraph::NodeId goal);

private:

	AirGraph& m_graph;
	const Dijkstra& m_dijkstra;
	std::vector<Restriction> m_restrictions; //!< we "use up" these restrictions in search
	Counter* m_counter; //!< Optional performance measurement dependency

};


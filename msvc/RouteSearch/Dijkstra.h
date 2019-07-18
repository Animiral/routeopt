#pragma once

#include "Graph.h"
#include "Cost.h"
#include "Path.h"
#include "Counter.h"

/**
 * Implementation of Dijkstra's algorithm for searching through the AirGraph structure.
 * The algorithm keeps track of performance metrics like node visits.
 */
class Dijkstra
{

public:

	/**
	 * Construct the search with its dependency.
	 */
	explicit Dijkstra(AirGraph const& graph, Cost const& cost) noexcept;

	/**
	 * Configure the search to count performance events.
	 */
	void SetCounter(Counter& counter);

	/**
	 * Return the cheapest path from the start node to the goal node.
	 * If no search was executed or the search was not successful, return an empty path.
	 */
	Path Run(AirGraph::NodeId start, AirGraph::NodeId goal) const;

private:

	AirGraph const& m_graph;
	Cost const& m_cost;
	Counter* m_counter; //!< Optional performance measurement dependency

};

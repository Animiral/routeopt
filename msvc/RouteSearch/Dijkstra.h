#pragma once

#include "Graph.h"
#include "Cost.h"

/**
 * Implementation of Dijkstra's algorithm for searching through the Graph structure.
 * The algorithm keeps track of performance metrics like node visits and observes restrictions (TODO).
 */
class Dijkstra
{

public:

	/**
	 * Construct the search with its dependency.
	 */
	Dijkstra(Graph const& graph, Cost const& cost);

	/**
	 * Find a path from the start node to the goal node.
	 * After this call, the result and auxiliary data is stored in this object.
	 */
	void run(Node start, Node goal);

	/**
	 * Return the path that was found in the last search run.
	 * If no search was executed or the search was not successful, return an empty path.
	 */
	Path const& result() const;

private:

	Graph const& m_graph;
	Cost const& m_cost;
	Path m_result; //!< Container for the result of the search run

};

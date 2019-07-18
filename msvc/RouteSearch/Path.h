#pragma once

#include <vector>
#include "Graph.h"
#include "Cost.h"

/**
 * Shared definition of a search result.
 *
 * A path consists of the nodes and edges that one would visit, in order,
 * when going from the start (node 0) to the goal (node N).
 *
 * The edges [0 - 1-N] are the transitions between the nodes, such that
 * `graph.GetEdge(edges[i]).from == nodes[i]` and
 * `graph.GetEdge(edges[i]).to == nodes[i+1]`.
 */
struct Path
{
	std::vector<AirGraph::NodeId> nodes;
	std::vector<AirGraph::EdgeId> edges;
	CostValue cost; //!< total cost of this solution
};

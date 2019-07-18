#include "Dijkstra.h"
#include <vector>
#include <set>
#include <algorithm>

namespace
{

// shortcuts
using NodeId = AirGraph::NodeId;
using EdgeId = AirGraph::EdgeId;

/**
 * Represents a Node in our "open" set which we might explore.
 */
struct OpenNode
{
	NodeId node; //!< node data
	CostValue cost; //!< cumulative cost to reach this Waypoint
	NodeId previous; //!< backtracking info for result

	/**
	 * This is a requirement for using OpenNode in std::upper_bound.
	 * Nodes are sorted in descending order of cost, such that the back()
	 * of a sorted vector will hold the next interesting open node.
	 */
	bool operator<(OpenNode rhs) const noexcept
	{
		return this->cost > rhs.cost;
	}
};

/**
 * This type of container holds the nodes already visited.
 *
 * Its associated edge points to the node from the previous node
 * along the shortest path.
 */
using ClosedSet = std::unordered_map<NodeId, EdgeId>;

/**
 * After the algorithm has found the shortest path to every node in the
 * closed set, construct the shortest path to the goal from the backtrack
 * information.
 *
 * @param graph the containing graph
 * @param closedSet map of path Waypoints to predecessors
 * @param start the terminal node for backtracking
 * @param goal the final node which must exist in the closed set
 */
Path BacktrackResult(AirGraph const& graph, ClosedSet closedSet, NodeId start, NodeId goal);

}

Dijkstra::Dijkstra(AirGraph const& graph, Cost const& cost)
	:
	m_graph(graph),
	m_cost(cost),
	m_counter(nullptr)
{
}

void Dijkstra::SetCounter(Counter& counter)
{
	m_counter = &counter;
}

void Dijkstra::Run(NodeId start, NodeId goal)
{
	// clear slate
	m_result = {};

	std::vector<OpenNode> open; // path tips to explore in descending order of cost
	ClosedSet closed; // map of path Waypoints to predecessors
	OpenNode current; // Waypoint currently under examination for shortest path

	open.push_back({start, CostValue{}, {}}); // start with 0 cost

	while(!open.empty())
	{
		// examine the nearest open node
		current = open.back();
		open.pop_back();

		if(m_counter) m_counter->NodeVisited();

		if(closed.end() != closed.find(current.node))
			break; // node already visited

		closed[current.node] = current.previous; // record backtrack

		if(current.node == goal)
		{
			m_result = BacktrackResult(m_graph, closed, start, goal);
			if(m_counter) m_counter->PathFound();
			break;
		}

		// explore all neighbors for paths
		for(AirGraph::EdgeId edge_id : m_graph.GetNode(current.node).out_edges)
		{
			AirGraph::Edge const& edge = m_graph.GetEdge(edge_id);
			if(closed.end() != closed.find(edge.to))
				continue; // node already visited

			CostValue nextCost = current.cost + m_cost.Calculate(edge.airway);
			OpenNode nextOpen{edge.to, nextCost, edge_id};
			auto it = std::lower_bound(open.begin(), open.end(), nextOpen);
			open.insert(it, nextOpen);
			
			if(m_counter) m_counter->EdgeVisited();
		}
	}
}

Path const& Dijkstra::Result() const
{
	return m_result;
}

namespace
{

Path BacktrackResult(AirGraph const& graph, ClosedSet closedSet, NodeId start, NodeId goal)
{
	Path path;

	NodeId n;
	EdgeId e;
	for(n = goal, e = closedSet.at(n);
		!(start == n);
		n = graph.GetEdge(e).from, e = closedSet.at(n)) {

		path.nodes.push_back(n);
		path.edges.push_back(e);
	}
	path.nodes.push_back(start);

	std::reverse(path.nodes.begin(), path.nodes.end());
	std::reverse(path.edges.begin(), path.edges.end());

	return path;
}

}

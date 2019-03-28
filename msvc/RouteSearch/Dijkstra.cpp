#include "Dijkstra.h"
#include <vector>
#include <set>
#include <algorithm>

namespace
{

/**
 * Represents a Node in our "open" set which we might explore.
 */
struct OpenNode
{
	Node node; //!< node data
	CostValue cost; //!< cumulative cost to reach this Node
	Node previous; //!< backtracking info for result

	/**
	 * This is a requirement for using OpenNode in std::lower_bound.
	 */
	bool operator<(OpenNode rhs) const noexcept
	{
		return this->cost < rhs.cost;
	}
};

/**
 * This type of container holds the nodes already visited.
 * It associates to the previous node along the shortest path.
 */
using ClosedSet = std::unordered_map<Node, Node, NodeHash>;

/**
 * After the algorithm has found the shortest path to every node in the
 * closed set, construct the shortest path to the goal from the backtrack
 * information.
 * @param closedSet map of path Nodes to predecessors
 * @param start the terminal node for backtracking
 * @param goal the final node which must exist in the closed set
 */
Path BacktrackResult(ClosedSet closedSet, Node start, Node goal);

}

Dijkstra::Dijkstra(Graph const& graph, Cost const& cost) : m_graph(graph), m_cost(cost)
{
}

void Dijkstra::run(Node start, Node goal)
{
	// clear slate
	m_result = {};

	std::vector<OpenNode> open; // path tips to explore in descending order of cost
	ClosedSet closed; // map of path Nodes to predecessors
	OpenNode current; // Node currently under examination for shortest path

	open.push_back({start, CostValue{}, {}}); // start with 0 cost

	while(!open.empty())
	{
		// examine the nearest open node
		current = open.back();
		open.pop_back();

		if(closed.end() != closed.find(current.node))
			break; // node already visited

		closed[current.node] = current.previous; // record backtrack

		if(current.node == goal)
		{
			m_result = BacktrackResult(closed, start, goal);
			break;
		}

		// explore all neighbors for paths
		for(Node next : m_graph.Neighbors(current.node))
		{
			if(closed.end() != closed.find(next))
				break; // node already visited

			CostValue nextCost = m_cost.Calculate({current.node, next});
			OpenNode nextOpen{next, nextCost, current.node};
			auto it = std::lower_bound(open.begin(), open.end(), nextOpen);
			open.insert(it, nextOpen);
		}
	}
}

Path const& Dijkstra::result() const
{
	return m_result;
}

namespace
{

Path BacktrackResult(ClosedSet closedSet, Node start, Node goal)
{
	Path path;

	for(Node n = goal; !(start == n); n = closedSet.at(n)) {
		path.push_back(n);
	}

	path.push_back(start);
	std::reverse(path.begin(), path.end());
	return path;
}

}
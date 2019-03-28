/// Contains the basic graph structures that we use for searching.
#pragma once

#include <vector>
#include <unordered_map>

/**
 * The nodes of the Airspace proto-graph.
 */
struct Node
{
	float latitude;
	float longitude;
	int flightlevel;
};

/**
 * Two nodes are equal if all their attributes are equal.
 */
bool operator==(Node lhs, Node rhs) noexcept;

/**
 * The edges of the Airspace proto-graph.
 */
struct Edge
{
	Node from;
	Node to;
};

/**
 * The Airspace proto-graph.
 * In this graph, the nodes do not contain flight-specific attributes.
 */
class Graph
{

public:

	explicit Graph();

	void AddNode(Node node);
	void AddEdge(Edge edge);

	/**
	 * Return all nodes in the Graph.
	 */
	std::vector<Node> const& Nodes() const noexcept;

	/**
	 * Return all nodes for the given node that are connected via an edge.
	 * If the node does not exist in the graph, throw std::out_of_range.
	 */
	std::vector<Node> const& Neighbors(Node node) const;

private:

	std::vector<Node> m_nodes;

	// Requirement for using Node as key in std::unordered_map
	struct NodeHash
	{
		size_t operator()(Node node) const noexcept;
	};

	std::unordered_map<Node, std::vector<Node>, NodeHash> m_edges;

};


/**
 * Shared definition of a search result.
 */
using Path = std::vector<Node>;

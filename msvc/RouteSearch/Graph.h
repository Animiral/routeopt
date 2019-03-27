/// Contains the basic graph structures that we use for searching.
#pragma once

#include <vector>

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
 * The edges of the Airspace proto-graph.
 */
struct Edge
{
	Node& from;
	Node& to;
};

/**
 * The Airspace proto-graph.
 * In this graph, the nodes do not contain flight-specific attributes.
 */
class Graph
{

public:

	explicit Graph();

	Node& AddNode(Node node);
	void AddEdge(Edge edge);

	std::vector<Node> const& Nodes() const;

private:

	std::vector<Node> m_nodes;

};


/**
 * Shared definition of a search result.
 */
using Path = std::vector<Node>;

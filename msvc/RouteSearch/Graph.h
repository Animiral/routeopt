#pragma once

#include <vector>

// Shadow graph nodes and edges
struct Node
{
	float latitude;
	float longitude;
	int flightlevel;
};

struct Edge
{
	Node& from;
	Node& to;
};

// Airspace "shadow" graph representation
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

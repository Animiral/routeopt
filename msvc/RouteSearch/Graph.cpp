#include "Graph.h"

// constructing shit
Graph::Graph() = default;

Node& Graph::AddNode(Node node)
{
	return m_nodes.emplace_back(std::move(node));
}

void Graph::AddEdge(Edge edge)
{

}


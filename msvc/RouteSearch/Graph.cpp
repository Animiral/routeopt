#include "Graph.h"

bool operator==(Node lhs, Node rhs) noexcept
{
	return
		lhs.latitude == rhs.latitude &&
		lhs.longitude == rhs.longitude &&
		lhs.flightlevel == rhs.flightlevel;
}

Graph::Graph() = default;

void Graph::AddNode(Node node)
{
	return m_nodes.push_back(node);
}

void Graph::AddEdge(Edge edge)
{
	// TODO: implementation
}

std::vector<Node> const& Graph::Nodes() const noexcept
{
	return m_nodes;
}

std::vector<Node> const& Graph::Neighbors(Node node) const
{
	return m_edges.at(node);
}

size_t Graph::NodeHash::operator()(Node node) const noexcept
{
	// based on sensible ranges for node attributes
	return static_cast<size_t>(((node.latitude * 180) + node.longitude) * 1000 + node.flightlevel);
}

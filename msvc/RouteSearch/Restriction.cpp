#include "Restriction.h"
#include <algorithm>

Restriction::Restriction(AirGraph::NodeId a, AirGraph::NodeId b) noexcept
	: m_a(a), m_b(b)
{
}

bool Restriction::CheckViolated(const Path& path) const noexcept
{
	return std::find(path.nodes.begin(), path.nodes.end(), m_a) != path.nodes.end()
		&& std::find(path.nodes.begin(), path.nodes.end(), m_b) != path.nodes.end();
}

std::pair<AirGraph, AirGraph> Restriction::Prevent(AirGraph graph)
{
	AirGraph graph2{graph}; // copy
	graph.RemoveNode(m_a);
	graph2.RemoveNode(m_b);
	return {std::move(graph), std::move(graph2)};
}

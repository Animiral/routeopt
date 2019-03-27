#include "Dijkstra.h"

Dijkstra::Dijkstra(Graph const& graph) : m_graph(graph)
{
}

void Dijkstra::run(Node const& start, Node const& goal)
{
	// TODO: implementation
}

Path const& Dijkstra::result() const
{
	return m_result;
}

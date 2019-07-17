#include "Counter.h"

Counter::Counter()
	:
	m_node_visited(0),
	m_edge_visited(0),
	m_path_found(0),
	m_restriction_violated(0)
{
}

void Counter::NodeVisited()
{
	m_node_visited++;
}

void Counter::EdgeVisited()
{
	m_edge_visited++;
}

void Counter::PathFound()
{
	m_path_found++;
}

void Counter::RestrictionViolated()
{
	m_restriction_violated++;
}

long Counter::CountNodeVisited()
{
	return m_node_visited;
}

long Counter::CountEdgeVisited()
{
	return m_edge_visited;
}

long Counter::CountPathFound()
{
	return m_path_found;
}

long Counter::CountRestrictionViolated()
{
	return m_restriction_violated;
}

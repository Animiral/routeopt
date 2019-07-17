#include "Counter.h"
#include <sstream>

Counter::Counter() noexcept
	:
	m_node_visited(0),
	m_edge_visited(0),
	m_path_found(0),
	m_restriction_violated(0)
{
}

std::string Counter::ToCsv() const
{
	std::stringstream ss;

	ss << std::to_string(m_node_visited) << ","
		<< std::to_string(m_edge_visited) << ","
		<< std::to_string(m_path_found) << ","
		<< std::to_string(m_restriction_violated);

	return ss.str();
}

void Counter::NodeVisited() noexcept
{
	m_node_visited++;
}

void Counter::EdgeVisited() noexcept
{
	m_edge_visited++;
}

void Counter::PathFound() noexcept
{
	m_path_found++;
}

void Counter::RestrictionViolated() noexcept
{
	m_restriction_violated++;
}

long Counter::CountNodeVisited() noexcept
{
	return m_node_visited;
}

long Counter::CountEdgeVisited() noexcept
{
	return m_edge_visited;
}

long Counter::CountPathFound() noexcept
{
	return m_path_found;
}

long Counter::CountRestrictionViolated() noexcept
{
	return m_restriction_violated;
}

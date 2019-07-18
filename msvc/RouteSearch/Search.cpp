#include <algorithm>
#include "Search.h"

Search::Search(AirGraph& graph, const Dijkstra& dijkstra, std::vector<Restriction> restrictions) noexcept
	:
	m_graph(graph),
	m_dijkstra(dijkstra),
	m_restrictions(std::move(restrictions)),
	m_counter(nullptr)
{
}

void Search::SetCounter(Counter& counter)
{
	m_counter = &counter;
}

Path Search::Run(AirGraph::NodeId start, AirGraph::NodeId goal)
{
	Path result = m_dijkstra.Run(start, goal); // candidate solution

	auto violation_it = std::find_if(m_restrictions.begin(), m_restrictions.end(),
				 [&result](const Restriction& r) { return r.CheckViolated(result); });

	if(m_restrictions.end() == violation_it) { // no violation
		return result;
	}

	if(m_counter) m_counter->RestrictionViolated();

	// resolve the violation
	Restriction violation{*violation_it}; // preserve copy
	m_restrictions.erase(violation_it);

	m_graph.SetNodeDisabled(violation.a, true);
	Path result1 = Run(start, goal);
	m_graph.SetNodeDisabled(violation.a, false);
	
	m_graph.SetNodeDisabled(violation.a, true);
	Path result2 = Run(start, goal);
	m_graph.SetNodeDisabled(violation.b, false);

	return result1.cost < result2.cost ? result1 : result2;
}

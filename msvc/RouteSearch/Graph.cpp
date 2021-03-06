#include "Graph.h"
#include <algorithm>

bool operator==(Waypoint lhs, Waypoint rhs) noexcept
{
	return
		lhs.name == rhs.name &&
		lhs.latitude == rhs.latitude &&
		lhs.longitude == rhs.longitude &&
		lhs.flightlevel == rhs.flightlevel;
}

bool operator==(Airway lhs, Airway rhs) noexcept
{
	return
		lhs.name == rhs.name &&
		lhs.grossmass == rhs.grossmass &&
		lhs.distance == rhs.distance &&
		lhs.time == rhs.time &&
		lhs.fuel == rhs.fuel;
}

AirGraph::AirGraph() = default;

AirGraph::NodeId AirGraph::AddNode(Waypoint waypoint)
{
	NodeId id = m_nodes.size();
	m_nodes.push_back({id, false, waypoint, {}});
	return id;
}

void AirGraph::SetNodeDisabled(NodeId id, bool disabled)
{
	m_nodes.at(id).disabled = disabled;
}

AirGraph::EdgeId AirGraph::AddEdge(NodeId from, NodeId to, Airway airway)
{
	if(from >= m_nodes.size() || to >= m_nodes.size()) {
		throw std::out_of_range("AddEdge: node id out of range");
	}

	EdgeId id = m_edges.size();
	m_edges.push_back({id, from, to, airway});
	m_nodes[from].out_edges.push_back(id);
	return id;
}

AirGraph::Node const& AirGraph::GetNode(NodeId id) const
{
	return m_nodes.at(id);
}

AirGraph::Node const* AirGraph::FindNodeByName(std::string name) const noexcept
{
	auto it = std::find_if(m_nodes.begin(), m_nodes.end(), [&name](Node const& n) { return n.waypoint.name == name; });
	return m_nodes.end() == it ? nullptr : &*it;
}

std::vector<AirGraph::Node> const& AirGraph::GetNodes() const noexcept
{
	return m_nodes;
}

AirGraph::Edge const& AirGraph::GetEdge(EdgeId id) const
{
	return m_edges.at(id);
}

AirGraph::Edge const* AirGraph::FindEdgeByName(std::string name) const noexcept
{
	auto it = std::find_if(m_edges.begin(), m_edges.end(), [&name](Edge const& e) { return e.airway.name == name; });
	return m_edges.end() == it ? nullptr : &*it;
}

std::vector<AirGraph::Edge> const AirGraph::GetEdges() const
{
	return m_edges;
}

// *** GraphML input ***
// The following functions enable us to read a file in GraphML format

#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graphml.hpp>

namespace
{

struct GmlWaypoint
{
	std::string name;
	float latitude;
	float longitude;
	int flightlevel;
};

struct GmlAirway
{
	std::string name;
	int grossmass;
	float distance;
	float time;
	float fuel;
};

using RawGraph = boost::adjacency_list<boost::mapS, boost::vecS,
	boost::undirectedS, GmlWaypoint, GmlAirway>;
using VertexDescriptor = boost::graph_traits<RawGraph>::vertex_descriptor;
using EdgeDescriptor = boost::graph_traits<RawGraph>::edge_descriptor;

Waypoint ConvertWaypoint(VertexDescriptor vertexDescriptor, const RawGraph& rawGraph)
{
	Waypoint waypoint;
	waypoint.name = boost::get(&GmlWaypoint::name, rawGraph, vertexDescriptor);
	waypoint.latitude = boost::get(&GmlWaypoint::latitude, rawGraph, vertexDescriptor);
	waypoint.longitude = boost::get(&GmlWaypoint::longitude, rawGraph, vertexDescriptor);
	waypoint.flightlevel = boost::get(&GmlWaypoint::flightlevel, rawGraph, vertexDescriptor);
	return waypoint;
}

std::tuple<AirGraph::NodeId, AirGraph::NodeId, Airway> ConvertAirway(EdgeDescriptor edgeDescriptor, const RawGraph& rawGraph)
{
	Airway airway;
	airway.name = boost::get(&GmlAirway::name, rawGraph, edgeDescriptor);
	airway.grossmass = boost::get(&GmlAirway::grossmass, rawGraph, edgeDescriptor);
	airway.distance = boost::get(&GmlAirway::distance, rawGraph, edgeDescriptor);
	airway.time = boost::get(&GmlAirway::time, rawGraph, edgeDescriptor);
	airway.fuel = boost::get(&GmlAirway::fuel, rawGraph, edgeDescriptor);
	VertexDescriptor from = boost::source(edgeDescriptor, rawGraph);
	VertexDescriptor to = boost::target(edgeDescriptor, rawGraph);
	return {from, to, airway};
}

}

AirGraph AirGraph::FromGraphML(std::string graphText)
{
	RawGraph rawGraph;
	std::istringstream stream(graphText);

	boost::dynamic_properties properties;
	properties.property("name", boost::get(&GmlWaypoint::name, rawGraph));
	properties.property("latitude", boost::get(&GmlWaypoint::latitude, rawGraph));
	properties.property("longitude", boost::get(&GmlWaypoint::longitude, rawGraph));
	properties.property("flightlevel", boost::get(&GmlWaypoint::flightlevel, rawGraph));
	properties.property("name", boost::get(&GmlAirway::name, rawGraph));
	properties.property("grossmass", boost::get(&GmlAirway::grossmass, rawGraph));
	properties.property("distance", boost::get(&GmlAirway::distance, rawGraph));
	properties.property("time", boost::get(&GmlAirway::time, rawGraph));
	properties.property("fuel", boost::get(&GmlAirway::fuel, rawGraph));

	boost::read_graphml(stream, rawGraph, properties);

	// transfer the input boost graph into our own graph representation
	AirGraph graph;

	//boost::read_graphml(instream, adjlist, dp);
	auto[vertices_begin, vertices_end] = boost::vertices(rawGraph);

	for(auto v_it = vertices_begin; v_it != vertices_end; ++v_it) {
		Waypoint waypoint = ConvertWaypoint(*v_it, rawGraph);
		graph.AddNode(waypoint);
	}

	auto[edges_begin, edges_end] = boost::edges(rawGraph);
	
	for(auto e_it = edges_begin; e_it != edges_end; ++e_it) {
		auto [from, to, airway] = ConvertAirway(*e_it, rawGraph);
		graph.AddEdge(from, to, airway);
	}

	return graph;
}

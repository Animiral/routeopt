#include "Graph.h"
#include <algorithm>

bool operator==(Waypoint lhs, Waypoint rhs) noexcept
{
	return
		lhs.latitude == rhs.latitude &&
		lhs.longitude == rhs.longitude &&
		lhs.flightlevel == rhs.flightlevel;
}

size_t WaypointHash::operator()(Waypoint node) const noexcept
{
	// based on sensible ranges for node attributes
	return static_cast<size_t>(((node.latitude * 180) + node.longitude) * 1000 + node.flightlevel);
}

AirGraph::AirGraph() = default;

void AirGraph::AddWaypoint(Waypoint waypoint)
{
	m_waypoints.push_back(waypoint);
	m_outgoing[waypoint]; // inserts empty vector
}

void AirGraph::AddAirway(Airway airway)
{
	(void) m_outgoing.at(airway.to); // explicit bounds check for target Waypoint
	m_outgoing.at(airway.from).push_back(airway);
}

std::vector<Waypoint> const& AirGraph::Waypoints() const noexcept
{
	return m_waypoints;
}

std::vector<Airway> const& AirGraph::Outgoing(Waypoint waypoint) const
{
	return m_outgoing.at(waypoint);
}

std::vector<Waypoint> const AirGraph::Neighbors(Waypoint waypoint) const
{
	std::vector<Waypoint> neighbors;
	auto& outWays = Outgoing(waypoint);
	std::transform(outWays.begin(), outWays.end(), std::back_inserter(neighbors), [](const Airway& a) { return a.to; });
	return neighbors;
}

#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graphml.hpp>

namespace
{

struct GmlWaypoint
{
	float latitude;
	float longitude;
	int flightlevel;
};

struct GmlAirway
{
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
	waypoint.latitude = boost::get(&GmlWaypoint::latitude, rawGraph, vertexDescriptor);
	waypoint.longitude = boost::get(&GmlWaypoint::longitude, rawGraph, vertexDescriptor);
	waypoint.flightlevel = boost::get(&GmlWaypoint::flightlevel, rawGraph, vertexDescriptor);
	return waypoint;
}

Airway ConvertAirway(EdgeDescriptor edgeDescriptor, const RawGraph& rawGraph)
{
	Airway airway;
	airway.grossmass = boost::get(&GmlAirway::grossmass, rawGraph, edgeDescriptor);
	airway.distance = boost::get(&GmlAirway::distance, rawGraph, edgeDescriptor);
	airway.time = boost::get(&GmlAirway::time, rawGraph, edgeDescriptor);
	airway.fuel = boost::get(&GmlAirway::fuel, rawGraph, edgeDescriptor);
	VertexDescriptor from = boost::source(edgeDescriptor, rawGraph);
	airway.from = ConvertWaypoint(from, rawGraph);
	VertexDescriptor to = boost::target(edgeDescriptor, rawGraph);
	airway.to = ConvertWaypoint(to, rawGraph);
	return airway;
}

}

AirGraph AirGraph::FromGraphML(std::string graphText)
{
	RawGraph rawGraph;
	std::istringstream stream(graphText);

	boost::dynamic_properties properties;
	properties.property("latitude", boost::get(&GmlWaypoint::latitude, rawGraph));
	properties.property("longitude", boost::get(&GmlWaypoint::longitude, rawGraph));
	properties.property("flightlevel", boost::get(&GmlWaypoint::flightlevel, rawGraph));
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
		graph.AddWaypoint(waypoint);
	}

	auto[edges_begin, edges_end] = boost::edges(rawGraph);
	
	for(auto e_it = edges_begin; e_it != edges_end; ++e_it) {
		Airway airway = ConvertAirway(*e_it, rawGraph);
		graph.AddAirway(airway);
	}

	return graph;
}

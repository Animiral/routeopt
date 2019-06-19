/// Contains the basic graph structures that we use for searching.
#pragma once

#include <vector>
#include <unordered_map>

/**
 * The nodes of the airspace proto-graph.
 */
struct Waypoint
{
	float latitude;
	float longitude;
	int flightlevel;
};

/**
 * Two waypoints are equal if all their attributes are equal.
 */
bool operator==(Waypoint lhs, Waypoint rhs) noexcept;

/**
 * This helper struct is a requirement for using Waypoint as key in std::unordered_map.
 */
struct WaypointHash
{
	size_t operator()(Waypoint node) const noexcept;
};

/**
 * The edges of the airspace proto-graph.
 */
struct Airway
{
	Waypoint from;
	Waypoint to;

	int grossmass;
	float distance;
	float time;
	float fuel;
};

/**
 * The AirGraph proto-graph.
 * In this graph, the nodes do not contain flight-specific attributes.
 */
class AirGraph
{

public:

	explicit AirGraph();

	void AddWaypoint(Waypoint node);

	/**
	 * Add the Airway to the AirGraph.
	 * @throw std::out_of_range if the to-Waypoint is not in the graph.
	 */
	void AddAirway(Airway edge);

	/**
	 * Return all waypoints in the AirGraph.
	 */
	std::vector<Waypoint> const& Waypoints() const noexcept;

	/**
	 * Return all airways that lead from the specified waypoint.
	 */
	std::vector<Airway> const& Outgoing(Waypoint waypoint) const;

	/**
	 * Return all waypoints for the given waypoint that are connected via an airway.
	 * @throw std::out_of_range if the waypoint does not exist in the graph.
	 */
	std::vector<Waypoint> const Neighbors(Waypoint node) const;

	/**
	 * Return the AirGraph that is specified in the given GraphML representation.
	 * The AirGraph must conform to certain expectations for vertex and edge
	 * properties:
	 * <key id="lat" for="node" attr.name="latitude" attr.type="float" />
	 * <key id="lng" for="node" attr.name="longitude" attr.type="float" />
	 * <key id="fl" for="node" attr.name="flightlevel" attr.type="int" />
	 * <key id="gm" for="edge" attr.name="grossmass" attr.type="int" />
	 * <key id="dist" for="edge" attr.name="distance" attr.type="float" />
	 * <key id="time" for="edge" attr.name="time" attr.type="float" />
	 * <key id="fuel" for="edge" attr.name="fuel" attr.type="float" />
	 */
	static AirGraph FromGraphML(std::string graphText);

private:

	std::vector<Waypoint> m_waypoints;
	std::unordered_map<Waypoint, std::vector<Airway>, WaypointHash> m_outgoing; //!< Outgoing airways from a waypoint

};


/**
 * Shared definition of a search result.
 */
using Path = std::vector<Waypoint>;

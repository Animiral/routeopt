/// Contains the basic graph structures that we use for searching.
#pragma once

#include <vector>
#include <string>

/**
 * The node attributes of the airspace proto-graph.
 */
struct Waypoint
{
	std::string name;
	float latitude;
	float longitude;
	int flightlevel;
};

/**
 * Two waypoints are equal if all their attributes are equal.
 */
bool operator==(Waypoint lhs, Waypoint rhs) noexcept;

/**
 * The edge attributes of the airspace proto-graph.
 */
struct Airway
{
	std::string name;
	int grossmass;
	float distance;
	float time;
	float fuel;
};

/**
 * Two airways are equal if all their attributes are equal.
 */
bool operator==(Airway lhs, Airway rhs) noexcept;

/**
 * The AirGraph proto-graph.
 * It is a directed graph of Airways between Waypoints.
 * Represents the search space for flight routes.
 */
class AirGraph
{

public:

	// Types: data contained in the graph is represented like this.
	using NodeId = size_t; // index into node container
	using EdgeId = size_t; // index into edge container

	struct Node
	{
		NodeId id; //!< unique node identifier number
		bool disabled; //!< flag to remove the node from search
		Waypoint waypoint; //!< Node attribute data
		std::vector<EdgeId> out_edges; //!< list of connected edges
	};

	struct Edge
	{
		EdgeId id;
		NodeId from;
		NodeId to;
		Airway airway;
	};

	explicit AirGraph();

	/**
	 * Add the Waypoint to the AirGraph.
	 * @return the id of the resultant Node.
	 */
	NodeId AddNode(Waypoint waypoint);

	/**
	 * Edit the disabled status of the specified waypoint in the AirGraph.
	 */
	void SetNodeDisabled(NodeId id, bool disabled);

	/**
	 * Add the Airway to the AirGraph.
	 *
	 * @return the id of the resultant Edge.
	 * @throw std::out_of_range if one of the nodes is not in the graph.
	 */
	EdgeId AddEdge(NodeId from, NodeId to, Airway airway);

	/**
	 * Return one node in the AirGraph.
	 *
	 * Caution! This reference may become invalid when the graph is mutated.
	 *
	 * @throw std::out_of_range if there is no node with the given id in the graph.
	 */
	Node const& GetNode(NodeId id) const;

	/**
	 * Return a pointer to a node with the specified name in the AirGraph.
	 * If no such node exists, return nullptr.
	 *
	 * Caution! This reference may become invalid when the graph is mutated.
	 */
	Node const* FindNodeByName(std::string name) const noexcept;

	/**
	 * Return all nodes in the AirGraph.
	 */
	std::vector<Node> const& GetNodes() const noexcept;

	/**
	 * Return one edge in the AirGraph.
	 *
	 * Caution! This reference may become invalid when the graph is mutated.
	 *
	 * @throw std::out_of_range if there is no edge with the given id in the graph.
	 */
	Edge const& GetEdge(EdgeId id) const;

	/**
	 * Return a pointer to an edge with the specified name in the AirGraph.
	 * If no such edge exists, return nullptr.
	 *
	 * Caution! This reference may become invalid when the graph is mutated.
	 */
	Edge const* FindEdgeByName(std::string name) const noexcept;

	/**
	 * Return all edges in the AirGraph.
	 */
	std::vector<Edge> const GetEdges() const;

	/**
	 * Return the AirGraph that is specified in the given GraphML representation.
	 * The AirGraph must conform to certain expectations for vertex and edge
	 * properties:
	 * &lt;key id="lat" for="node" attr.name="latitude" attr.type="float" />
	 * &lt;key id="lng" for="node" attr.name="longitude" attr.type="float" />
	 * &lt;key id="fl" for="node" attr.name="flightlevel" attr.type="int" />
	 * &lt;key id="gm" for="edge" attr.name="grossmass" attr.type="int" />
	 * &lt;key id="dist" for="edge" attr.name="distance" attr.type="float" />
	 * &lt;key id="time" for="edge" attr.name="time" attr.type="float" />
	 * &lt;key id="fuel" for="edge" attr.name="fuel" attr.type="float" />
	 */
	static AirGraph FromGraphML(std::string graphText);

private:

	std::vector<Node> m_nodes;
	std::vector<Edge> m_edges;

};

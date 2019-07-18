/**
 * Contains handy graph objects to perform tests on.
 */
#pragma once

#include "Graph.h"
#include "Restriction.h"

/**
 * A small test graph with 5 nodes.
 *
 *   --- b ---
 *  /         \
 * a - c - d - e
 */
struct Scenario1
{
	explicit Scenario1();

	AirGraph graph;
	AirGraph::NodeId a_id, b_id, c_id, d_id, e_id;
	AirGraph::EdgeId ac_id, ab_id, be_id, cd_id, de_id;
};

/**
 * Another test graph with 6 nodes.
 *
 *  -->1-->3--->5-\
 * 0        \      -->4
 *  ----------->2-/
 */
struct Scenario2
{
	explicit Scenario2();

	AirGraph graph;
	AirGraph::NodeId a_id, b_id, c_id, d_id, e_id, f_id;
};

/**
 * A test graph with a restriction that will require multiple attempts.
 * The most efficient route is a, b, c, d.
 * Since (b, c) is restricted, the next cheaper option is a, c, d.
 */
	//  -5->b---30---\
	// a      \5      -->d
	//  ---20---->c-5/
struct Scenario3
{
	explicit Scenario3();

	AirGraph graph;
	AirGraph::NodeId a_id, b_id, c_id, d_id;
	AirGraph::EdgeId ab_id, ac_id, bc_id, bd_id, cd_id;
	Restriction restriction;
};

#include "Scenario.h"

Scenario1::Scenario1()
	:
	graph(),
	a_id(graph.AddNode({0, 0, 100})   ),
	b_id(graph.AddNode({100, 15, 100})),
	c_id(graph.AddNode({0, 10, 100})  ),
	d_id(graph.AddNode({0, 20, 100})  ),
	e_id(graph.AddNode({0, 30, 100})  )
{
	ab_id = graph.AddEdge(a_id, b_id, {20000, 150.f, .5f, 100.f});
	be_id = graph.AddEdge(b_id, e_id, {20000, 150.f, .5f, 100.f});
	ac_id = graph.AddEdge(a_id, c_id, {20000, 10.f, .5f, 100.f});
	cd_id = graph.AddEdge(c_id, d_id, {20000, 10.f, .5f, 100.f});
	de_id = graph.AddEdge(d_id, e_id, {20000, 200.f, .5f, 100.f});
}

Scenario2::Scenario2()
	:
	graph(),
	a_id(graph.AddNode({0, 0, 100})   ),
	b_id(graph.AddNode({100, -50, 100})),
	c_id(graph.AddNode({300, 50, 100})  ),
	d_id(graph.AddNode({200, -50, 100})  ),
	e_id(graph.AddNode({300, -50, 100})  ),
	f_id(graph.AddNode({400, 0, 100})  )
{
	graph.AddEdge(a_id, b_id, {20000, 10.f, .5f, 100.f});
	graph.AddEdge(a_id, c_id, {20000, 10.f, .5f, 100.f});
	graph.AddEdge(b_id, d_id, {20000, 10.f, .5f, 100.f});
	graph.AddEdge(d_id, c_id, {20000, 5.f, .5f, 100.f});
	graph.AddEdge(c_id, e_id, {20000, 10.f, .5f, 100.f});
	graph.AddEdge(d_id, f_id, {20000, 10.f, .5f, 100.f});
	graph.AddEdge(f_id, e_id, {20000, 10.f, .5f, 100.f});
}

#include "Scenario.h"

Scenario1::Scenario1()
	:
	graph(),
	a_id(graph.AddNode({"a", 0, 0, 100})   ),
	b_id(graph.AddNode({"b", 100, 15, 100})),
	c_id(graph.AddNode({"c", 0, 10, 100})  ),
	d_id(graph.AddNode({"d", 0, 20, 100})  ),
	e_id(graph.AddNode({"e", 0, 30, 100})  )
{
	ab_id = graph.AddEdge(a_id, b_id, {"ab", 20000, 150.f, .5f, 100.f});
	be_id = graph.AddEdge(b_id, e_id, {"be", 20000, 150.f, .5f, 100.f});
	ac_id = graph.AddEdge(a_id, c_id, {"ac", 20000, 10.f, .5f, 100.f});
	cd_id = graph.AddEdge(c_id, d_id, {"cd", 20000, 10.f, .5f, 100.f});
	de_id = graph.AddEdge(d_id, e_id, {"de", 20000, 200.f, .5f, 100.f});
}

Scenario2::Scenario2()
	:
	graph(),
	a_id(graph.AddNode({"a", 0, 0, 100})   ),
	b_id(graph.AddNode({"b", 100, -50, 100})),
	c_id(graph.AddNode({"c", 300, 50, 100})  ),
	d_id(graph.AddNode({"d", 200, -50, 100})  ),
	e_id(graph.AddNode({"e", 300, -50, 100})  ),
	f_id(graph.AddNode({"f", 400, 0, 100})  )
{
	graph.AddEdge(a_id, b_id, {"ab", 20000, 10.f, .5f, 100.f});
	graph.AddEdge(a_id, c_id, {"ac", 20000, 10.f, .5f, 100.f});
	graph.AddEdge(b_id, d_id, {"bg", 20000, 10.f, .5f, 100.f});
	graph.AddEdge(d_id, c_id, {"dc", 20000, 5.f, .5f, 100.f});
	graph.AddEdge(c_id, e_id, {"ce", 20000, 10.f, .5f, 100.f});
	graph.AddEdge(d_id, f_id, {"df", 20000, 10.f, .5f, 100.f});
	graph.AddEdge(f_id, e_id, {"fe", 20000, 10.f, .5f, 100.f});
}

Scenario3::Scenario3()
	:
	a_id(graph.AddNode({"a", 0, 0, 100})),
	b_id(graph.AddNode({"b", 10, -10, 110})),
	c_id(graph.AddNode({"c", 20, 10, 100})),
	d_id(graph.AddNode({"d", 30, 0, 110})),
	ab_id(graph.AddEdge(a_id, b_id, {"ab", 20000, 5.f, .5f, 100.f})),
	ac_id(graph.AddEdge(a_id, c_id, {"ac", 20000, 30.f, .5f, 100.f})),
	bc_id(graph.AddEdge(b_id, c_id, {"bc", 20000, 5.f, .5f, 100.f})),
	bd_id(graph.AddEdge(b_id, d_id, {"bd", 20000, 20.f, .5f, 100.f})),
	cd_id(graph.AddEdge(c_id, d_id, {"cd", 20000, 5.f, .5f, 100.f})),
	restriction{b_id, c_id}
{
}
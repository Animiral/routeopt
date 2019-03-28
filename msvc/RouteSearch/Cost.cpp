#include "Cost.h"

float Cost::Calculate(Edge edge) const
{
	// crappy stub implementation: flat earth + Manhattan distance
	return edge.to.latitude - edge.from.latitude
		+ edge.to.longitude - edge.from.longitude
		+ edge.to.flightlevel - edge.from.flightlevel;
}

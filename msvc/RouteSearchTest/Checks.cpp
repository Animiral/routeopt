#include "Checks.h"
#include <algorithm>

namespace
{

bool IsNodeEqual(Node const& lhs, Node const& rhs)
{
	return
		lhs.latitude == rhs.latitude &&
		lhs.longitude == rhs.longitude &&
		lhs.flightlevel == rhs.flightlevel;
}

}

bool IsPathEqual(const Path& expected, const Path& actual)
{
	return std::equal(expected.begin(), expected.end(), actual.begin(), actual.end(), IsNodeEqual);
}

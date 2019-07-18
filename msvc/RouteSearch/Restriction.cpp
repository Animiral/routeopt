#include "Restriction.h"
#include <algorithm>

bool Restriction::CheckViolated(const Path& path) const noexcept
{
	return std::find(path.nodes.begin(), path.nodes.end(), a) != path.nodes.end()
		&& std::find(path.nodes.begin(), path.nodes.end(), b) != path.nodes.end();
}

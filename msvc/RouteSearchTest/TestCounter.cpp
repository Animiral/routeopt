// Tests for the performance counter

#include "catch.hpp"
#include "Counter.h"

TEST_CASE("ToCsv")
{
	Counter c;
	for(int i = 0; i < 4; i++) c.NodeVisited();
	for(int i = 0; i < 3; i++) c.EdgeVisited();
	for(int i = 0; i < 2; i++) c.PathFound();
	for(int i = 0; i < 1; i++) c.RestrictionViolated();

	CHECK(c.ToCsv() == "4,3,2,1");
}

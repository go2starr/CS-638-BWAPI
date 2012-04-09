#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "MockUnit.h"


TEST(StrategizerTest, redistrubteAgents)
{
	BWAPI::MockUnit mUnit;

	Agent agent(&mUnit);

    EXPECT_EQ(4, 4);
}

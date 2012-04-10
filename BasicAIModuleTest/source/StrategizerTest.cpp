#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "MockUnit.h"
#include "Strategizer.h"
#include "SCVAgent.h"

TEST(StrategizerTest, redistrubteAgents)
{
	BWAPI::MockUnit mUnit;

	SCVAgent scv(mUnit);

    EXPECT_EQ(4, 4);
}

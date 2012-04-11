#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "BasicAIModule.h"
#include "MockUnit.h"
#include "Strategizer.h"
#include "ActorAgent.h"


TEST(StrategizerTest, agentConstructorTest)
{
	BWAPI::MockUnit mUnit;
	ActorAgent agent(mUnit);
    EXPECT_TRUE(&(agent.getUnit()) == &mUnit);
}


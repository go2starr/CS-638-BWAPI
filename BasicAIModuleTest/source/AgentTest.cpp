#include "gmock/gmock.h"
#include "BasicAIModule.h"
#include "MockUnit.h"
#include "Strategizer.h"
#include "ActorAgent.h"

#include <string>

using namespace BWAPI;
using std::string;

TEST(ActorAgentTest, agentConstructorTest)
{
    MockUnit mUnit;
    ActorAgent agent(mUnit);

    EXPECT_TRUE(&(agent.getUnit()) == &mUnit);
    EXPECT_TRUE(agent.getState() == IdleState);
    EXPECT_TRUE(agent.getUnitTarget() == NULL);
    EXPECT_TRUE(agent.getUnitTypeTarget() == UnitTypes::None);
    EXPECT_TRUE(agent.getParentManagerName() == string("None"));
}

TEST(ActorAgentTest, agentEqualityTest)
{
    MockUnit mUnit;
    ActorAgent agent1a(mUnit);
    ActorAgent agent1b(mUnit);

    EXPECT_TRUE(agent1a == agent1b);

    MockUnit mUnit2;
    ActorAgent agent2(mUnit2);

    EXPECT_FALSE(agent1a == agent2);
    EXPECT_FALSE(agent1b == agent2);
}

TEST(ActorAgentTest, agentStateTest)
{
    MockUnit mUnit;
    ActorAgent agent(mUnit);

    EXPECT_TRUE(agent.getState() == IdleState);
    for(int i = 0; i < NUM_STATES; ++i)
    {
        const State& state = static_cast<State>(i);
        agent.setState(state);
        EXPECT_TRUE(agent.getState() == state);
    }
}

TEST(ActorAgentTest, agentUnitTargetTest)
{
    MockUnit mUnit;
    ActorAgent agent(mUnit);

    EXPECT_TRUE(agent.getUnitTarget() == NULL);

    MockUnit mUnitTarget;
    Unit *unitTarget = dynamic_cast<Unit*>(&mUnitTarget);
    agent.setUnitTarget(unitTarget);

    EXPECT_TRUE(agent.getUnitTarget() == &mUnitTarget);
}

TEST(ActorAgentTest, agentUnitTypeTargetTest)
{
    MockUnit mUnit;
    ActorAgent agent(mUnit);

    EXPECT_TRUE(agent.getUnitTypeTarget() == UnitTypes::None);

    agent.setUnitTypeTarget(UnitTypes::Critter_Bengalaas);

    EXPECT_TRUE(agent.getUnitTypeTarget() == UnitTypes::Critter_Bengalaas);

    agent.setUnitTypeTarget(UnitTypes::Terran_Battlecruiser);

    EXPECT_TRUE(agent.getUnitTypeTarget() == UnitTypes::Terran_Battlecruiser);
}
/*
// NOTE:
// This test results in an access violation exception being thrown 
// because ActorAgent::isOccupiedPosition() uses Broodwar->foo()
// which isn't available because the dll isn't hooked into the 
// game when running unit tests...
// May have to mock out the Broodwar Game* to be able to unit test many things
TEST(ActorAgentTest, agentIsOccupiedPositionTest)
{
    MockUnit mUnit;
    ActorAgent agent(mUnit);
    Position target(-1,-1);

    EXPECT_FALSE(agent.isOccupiedPosition(target));

    Position target2(agent.getUnit().getPosition());

    EXPECT_TRUE(agent.isOccupiedPosition(target2));
}

TEST(ActorAgentTest, agentGetClosestPositionTest)
{
    // TODO...
}
*/

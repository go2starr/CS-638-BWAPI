#include "gmock/gmock.h"

#include "Manager.h"
#include "MockUnit.h"
#include "ActorAgent.h"
#include "Tasks/Task.h"

#include <vector>

using BWAPI::MockUnit;

// Allow tests access to protected things in Manager 
class TestableManager : public Manager
{
	public: 
		using Manager::agents;
		using Manager::tasks;
};


TEST(ManagerTest, addAgentTest)
{
	MockUnit unit1, unit2, unit3;
	ActorAgent agent1(unit1), agent2(unit2), agent3(unit3);

	TestableManager manager;
	EXPECT_TRUE(manager.numAgents() == 0);

	EXPECT_TRUE(manager.agents.find(&agent1) == manager.agents.end());
	EXPECT_TRUE(manager.agents.find(&agent2) == manager.agents.end());
	EXPECT_TRUE(manager.agents.find(&agent3) == manager.agents.end());

	manager.addAgent(agent1);
	EXPECT_TRUE(manager.numAgents() == 1);
	EXPECT_TRUE(manager.agents.find(&agent1) != manager.agents.end());

	manager.addAgent(agent2);
	EXPECT_TRUE(manager.numAgents() == 2);
	EXPECT_TRUE(manager.agents.find(&agent2) != manager.agents.end());

	manager.addAgent(agent3);
	EXPECT_TRUE(manager.numAgents() == 3);
	EXPECT_TRUE(manager.agents.find(&agent3) != manager.agents.end());
}

using ::testing::Return;

TEST(ManagerTest, removeAgentTest)
{
	MockUnit unit1, unit2, unit3;
	ActorAgent agent1(unit1), agent2(unit2), agent3(unit3);

	TestableManager manager;
	EXPECT_TRUE(manager.numAgents() == 0);

	manager.addAgent(agent1);
	manager.addAgent(agent2);
	manager.addAgent(agent3);
	EXPECT_TRUE(manager.numAgents() == 3);

	EXPECT_CALL(unit1, getType())
		.WillRepeatedly(Return(ByRef(BWAPI::UnitType(1))));
	EXPECT_CALL(unit2, getType())
		.WillRepeatedly(Return(ByRef(BWAPI::UnitType(2))));
	EXPECT_CALL(unit3, getType())
		.WillRepeatedly(Return(ByRef(BWAPI::UnitType(3))));

	const BWAPI::UnitType type1 = agent1.getUnit().getType();
	EXPECT_TRUE(manager.removeAgent(type1) == &agent1);
	EXPECT_TRUE(manager.numAgents() == 2);

	const BWAPI::UnitType type2 = agent2.getUnit().getType();
	EXPECT_TRUE(manager.removeAgent(type2) == &agent2);
	EXPECT_TRUE(manager.numAgents() == 1);

	const BWAPI::UnitType type3 = agent3.getUnit().getType();
	EXPECT_TRUE(manager.removeAgent(type3) == &agent3);
	EXPECT_TRUE(manager.numAgents() == 0);
}

TEST(ManagerTest, removeAllAgentsTest)
{
	MockUnit unit1, unit2, unit3;
	ActorAgent agent1(unit1), agent2(unit2), agent3(unit3);

	TestableManager manager;
	EXPECT_TRUE(manager.numAgents() == 0);

	manager.addAgent(agent1);
	manager.addAgent(agent2);
	manager.addAgent(agent3);
	EXPECT_TRUE(manager.numAgents() == 3);

	manager.removeAllAgents();
	EXPECT_TRUE(manager.numAgents() == 0);
}

TEST(ManagerTest, addTaskTest)
{
	TestableManager manager;
	Task task1, task2, task3;

	manager.addTask(task1);
	EXPECT_EQ(1, manager.tasks.size());
	manager.addTask(task2);
	EXPECT_EQ(2, manager.tasks.size());
	manager.addTask(task3);
	EXPECT_EQ(3, manager.tasks.size());
}

TEST(ManagerTest, numAgentsTest)
{
	const unsigned short NUMBER = 64;

	TestableManager manager;

	std::vector<MockUnit*>   units;
	std::vector<ActorAgent*> agents;
	for(unsigned short i = 0; i < NUMBER; ++i)
	{
		units.push_back(new MockUnit());
		agents.push_back(new ActorAgent(*units.back()));
		manager.addAgent(*agents.back());
	}
	EXPECT_EQ(NUMBER, manager.numAgents());

	for(unsigned short i = 0; i < NUMBER; ++i)
	{
		delete agents[i];
		delete units[i];
	}
}

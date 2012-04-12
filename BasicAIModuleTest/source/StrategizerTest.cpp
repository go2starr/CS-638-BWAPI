#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "Strategizer.h"
#include "Manager.h"
#include "MockUnit.h"
#include "ActorAgent.h"

using namespace BWAPI;


TEST(StrategizerTest, instanceTest)
{
	const Strategizer& strategizer1 = Strategizer::instance();
	const Strategizer& strategizer2 = Strategizer::instance();
	EXPECT_TRUE(&strategizer1 == &strategizer2 );  
}

/* problems...
TEST(StrategizerTest, remapTest)
{
	Strategizer *strategizer = &Strategizer::instance();

	class StrategizerInspector : public Strategizer
	{
		friend class StrategizerTest_remapTest_Test;
	};
	StrategizerInspector *strategizerPtr = reinterpret_cast<StrategizerInspector*>(strategizer);

	Manager *managerSrc = &(strategizerPtr->buildManager);
	Manager *managerDst = &(strategizerPtr->combatManager);

	class ManagerInspector : public Manager
	{
		friend class StrategizerTest_remapTest_Test;
	};
	ManagerInspector *managerSrcPtr = reinterpret_cast<ManagerInspector*>(managerSrc);
	ManagerInspector *managerDstPtr = reinterpret_cast<ManagerInspector*>(managerDst);

	MockUnit mUnit;
	ActorAgent agent(mUnit);
	strategizerPtr->unitAgentMap[&mUnit] = &agent;

	managerSrc->addAgent(agent);
	strategizerPtr->agentManagerMap[&Agent] = managerSrc;

	EXPECT_TRUE(managerSrcPtr->agents.find(&agent) != managerSrcPtr->agents.end());

	strategizerPtr->remap(UnitTypes::Terran_SCV
						, *managerSrc
						, *managerDst);
	//strategizerPtr->updateAgentManagerMap();

	EXPECT_TRUE(managerSrcPtr->agents.find(&agent) == managerSrcPtr->agents.end());
	EXPECT_TRUE(managerDstPtr->agents.find(&agent) != managerDstPtr->agents.end());
}
*/

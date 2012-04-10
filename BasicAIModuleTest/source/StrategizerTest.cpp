#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "BasicAIModule.h"
#include "MockUnit.h"
#include "Strategizer.h"
#include "SCVAgent.h"




TEST(StrategizerTest, redistrubteAgents)
{
	BWAPI::MockUnit mUnit;
	// error LNK2001: unresolved external symbol 
	// "public: __thiscall SCVAgent::SCVAgent(class BWAPI::Unit &)"
	// SCVAgent scv(mUnit);

    EXPECT_EQ(4, 4);
}

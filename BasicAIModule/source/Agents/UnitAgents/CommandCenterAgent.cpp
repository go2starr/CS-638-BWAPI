/*
 * CommandCenterAgent.cpp
 */
#include "UnitAgents/CommandCenterAgent.h"
#include "StructureAgent.h"

#include <BWAPI.h>

using namespace BWAPI;


CommandCenterAgent::CommandCenterAgent(Unit &u) 
	: StructureAgent(u)
{}

void CommandCenterAgent::update()
{
	switch (state)
	{
	case TrainState:
        if (!unit.isTraining() && !unit.isBeingConstructed() ) 
		{
			unit.train(UnitTypes::Terran_SCV);
		}
		break;
	}

	StructureAgent::update();
}

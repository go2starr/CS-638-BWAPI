/*
 * CommandCenterAgent.cpp
 */
#include "UnitAgents/CommandCenterAgent.h"
#include "StructureAgent.h"

#include <BWAPI.h>

using BWAPI::Unit;


CommandCenterAgent::CommandCenterAgent(Unit &u) 
	: StructureAgent(u)
{}

void CommandCenterAgent::update()
{
	if (state) {
		switch (state)
		{
		case TrainState:
			if (!unit.isTraining()) 
			{
				Broodwar->sendText("CC: Training SCV");
				unit.train(BWAPI::UnitTypes::Terran_SCV);
			}
			break;

		default: StructureAgent::update();
		}
	}
}

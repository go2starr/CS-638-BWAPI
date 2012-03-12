/*
 * CommandCenterAgent.cpp
 */
#include "UnitAgents/CommandCenterAgent.h"
#include "StructureAgent.h"

#include <BWAPI.h>

#include <sstream>
#include <string>

using BWAPI::Unit;
using BWAPI::Broodwar;

using std::stringstream;
using std::string;


CommandCenterAgent::CommandCenterAgent(Unit &u) 
	: StructureAgent(u)
{}

void CommandCenterAgent::update()
{
	switch (state)
	{
	case TrainState:
		if (!unit.isTraining()) 
		{
			unit.train(BWAPI::UnitTypes::Terran_SCV);
		}
		break;
	}

	StructureAgent::update();
}

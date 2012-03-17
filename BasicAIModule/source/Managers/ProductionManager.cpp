/*
 *  ProductionManager.cpp
 *  
 *  Used to train new units
 */
#include "ProductionManager.h"
#include "Agent.h"

#include <BWAPI.h>

using namespace BWAPI;


void ProductionManager::update()
{
	// currently once you put the command center in train state
	// it will never transition out of it
	// should tell it to be idle at some point, especially if
	// you want to build an addon
	for (AgentSetIter agent = agents.begin(); agent != agents.end(); agent++)
	{
		// Tell command centers to produce SCVs
		if ((*agent)->getUnit().getType().isResourceDepot())
		{
			(*agent)->setState(TrainState);
			(*agent)->setUnitTypeTarget(UnitTypes::Terran_SCV);
		}
	}

	// Update agents
	Manager::update();
}

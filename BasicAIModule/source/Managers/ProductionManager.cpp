#include "ProductionManager.h"

#include <BWAPI.h>
#include <set>

using namespace BWAPI;
using namespace std;

void ProductionManager::update()
{
	set<Agent*>::iterator agent;

	// currently once you put the command center in train state
	// it will never transition out of it
	// should tell it to be idle at some point, especially if
	// you want to build an addon
	for (agent = agents.begin(); agent != agents.end(); agent++)
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
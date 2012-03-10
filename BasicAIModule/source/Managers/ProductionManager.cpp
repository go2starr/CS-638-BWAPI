#include "ProductionManager.h"

#include <BWAPI.h>
#include <set>

using namespace BWAPI;
using namespace std;

void ProductionManager::update()
{
	set<Agent*>::iterator agent;
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
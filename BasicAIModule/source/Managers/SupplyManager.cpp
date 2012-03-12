#include "SupplyManager.h"
#include <BWAPI.h>

using namespace BWAPI;
using namespace std;

void SupplyManager::update()
{
	/* Build supply depots if we are running low */
	for (set<Agent*>::iterator it = agents.begin(); it != agents.end(); it++)
	{
		Agent *agent = *it;

		if (Broodwar->self()->supplyTotal() - Broodwar->self()->supplyUsed() < 8)
		{
			agent->setState(BuildState);
			agent->setUnitTypeTarget(BWAPI::UnitTypes::Terran_Supply_Depot);
		}
	}
	
	/* Base class updates Agents */
	Manager::update();
}
#include "SupplyManager.h"
#include <BWAPI.h>

using namespace BWAPI;
using namespace std;

void SupplyManager::update()
{
	Broodwar->sendText("SM: I have %d agents", agents.size());

	/* Build supply depots if we are running low */
	for (set<Agent*>::iterator it = agents.begin(); it != agents.end(); it++)
	{
		Agent *agent = *it;
		agent->setState(BuildState);
		agent->setUnitTypeTarget(BWAPI::UnitTypes::Terran_Supply_Depot);
	}
	
	/* Base class updates Agents */
	Manager::update();
}
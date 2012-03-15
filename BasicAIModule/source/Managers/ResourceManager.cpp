#include "ResourceManager.h"
#include <BWAPI.h>
#include <set>

using namespace BWAPI;
using namespace std;

void ResourceManager::update()
{
	Broodwar->drawTextScreen(2, 0, "\x1F RM : (SCV=%d)", numAgents(UnitTypes::Terran_SCV));

	// Send workers to mine minerals near our base
	set<Unit*> minerals = Broodwar->getMinerals();
	set<Unit*>::iterator mineral;
	AgentSetIter agent;

	for (agent = agents.begin(); agent != agents.end(); agent++)
	{
        const Unit& unit = (*agent)->getUnit();
		if (unit.getType().isWorker() && !unit.isGatheringGas())
		{
			int minDist = 9999999;
			Unit* closest = 0;
			for (mineral = minerals.begin(); mineral != minerals.end(); mineral++)
			{
				int dist = unit.getDistance(*mineral);
				if (dist < minDist)
				{
					closest = *mineral;
				}
			}
			if (closest)
			{
				(*agent)->setUnitTarget(closest);
				(*agent)->setState(GatherState);
			}
		}
	}
	
	// Update all agents
	Manager::update();
}

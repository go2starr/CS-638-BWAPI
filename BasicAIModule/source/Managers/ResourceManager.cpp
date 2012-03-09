#include "ResourceManager.h"
#include <BWAPI.h>
#include <set>

using namespace BWAPI;
using namespace std;

void ResourceManager::update()
{
	// Send workers to mine minerals near our base
	set<Unit*> minerals = Broodwar->getMinerals();
	set<Unit*>::iterator mineral;
	set<Agent*>::iterator agent;

	for (agent = agents.begin(); agent != agents.end(); agent++)
	{
		if ((*agent)->getUnit().getType().isWorker() &&
			(*agent)->getUnit().isIdle()) 
		{
			int minDist = 9999999;
			Unit* closest = 0;
			for (mineral = minerals.begin(); mineral != minerals.end(); mineral++)
			{
				int dist = (*agent)->getUnit().getDistance(*mineral);
				if (dist < minDist)
				{
					closest = *mineral;
				}
			}
			if (closest)
			{

			}
		}
	}
	
	// Update all agents
	Manager::update();
}
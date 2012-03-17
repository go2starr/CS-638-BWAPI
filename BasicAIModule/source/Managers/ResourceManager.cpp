#include "ResourceManager.h"
#include "Common.h"
#include "Agent.h"

#include <BWAPI.h>

#include <limits>

using namespace BWAPI;


void ResourceManager::update()
{
    // TODO: store the mineral units locally in ResourceMgr
    // so we can more effectively control which workers gather from which minerals
    
    Broodwar->drawTextScreen(2, 0, "\x1F RM : (SCV=%d)", numAgents(UnitTypes::Terran_SCV));

	// Send workers to mine minerals near our base
    UnitSet  minerals(Broodwar->getMinerals());
    AgentSet workers(getAgentsOfType(UnitTypes::Terran_SCV));
	for(AgentSetIter worker = workers.begin(); worker != workers.end(); ++worker)
	{
        int minDist = std::numeric_limits<int>::max();
		Unit* closest = NULL;
		for(UnitSetIter mineral = minerals.begin(); mineral != minerals.end(); mineral++)
		{
			int dist = (*worker)->getUnit().getDistance(*mineral);
			if (dist < minDist)
			{
                minDist = dist;
				closest = *mineral;
			}
		}
		if( closest != NULL )
		{
            (*worker)->setState(GatherState);
			(*worker)->setUnitTarget(closest);
            (*worker)->setUnitTypeTarget(closest->getType());
            (*worker)->setPositionTarget(closest->getPosition());
		}
	}
	
	// Update all agents
	Manager::update();
}

int ResourceManager::getMineralRate() const
{
    // TODO
}

int ResourceManager::getNumWorkersGathering() const
{
    int count = 0;
    AgentSetConstIter it  = agents.begin();
    AgentSetConstIter end = agents.end();
    for(; it != end;)
    {
        Agent *agent = *it;
        if( agent->getUnit().isGatheringMinerals() )
        {
            ++count;
        }
    }
    return count;    
}

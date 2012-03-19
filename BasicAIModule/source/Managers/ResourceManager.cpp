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

	// Send workers to mine minerals near our base
    AgentSet workers(getAgentsOfType(UnitTypes::Terran_SCV));
    for(AgentSetIter worker = workers.begin(); worker != workers.end(); ++worker)
    {
        makeAgentGatherMinerals(**worker);
    }
	
	// Update all agents
	Manager::update();
}

bool ResourceManager::makeAgentGatherMinerals(Agent& agent)
{
    bool success = false;

    Unit *closest = getClosestMineralPatch(agent);
    if( closest != NULL )
    {
        agent.setState(GatherState);
        agent.setUnitTarget(closest);
        agent.setUnitTypeTarget(closest->getType());
        agent.setPositionTarget(closest->getPosition());
        success = true;
    }

    return success;
}

Unit* ResourceManager::getClosestMineralPatch(const Agent& agent)
{
    Unit *closest = NULL;
    int   minDist = std::numeric_limits<int>::max();

    UnitSet minerals(Broodwar->getMinerals()); 
    for(UnitSetIter mineral = minerals.begin(); mineral != minerals.end(); ++mineral)
    {
        int dist = agent.getUnit().getDistance(*mineral);
        if (dist < minDist)
        {
            minDist = dist;
            closest = *mineral;
        }
    }

    return closest;
}

int ResourceManager::getMineralRate() const
{
    // TODO
	return 0;
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

void ResourceManager::draw()
{
	Broodwar->drawTextScreen(2, 0, "\x1F RM : (SCV=%d)", numAgents(UnitTypes::Terran_SCV));
	Manager::draw();
}
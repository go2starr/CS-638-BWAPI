/*
 *  ResourceAdvisor.cpp  
 *  
 *  This advisor provides info regarding map resources
 */
#include "ResourceAdvisor.h"
#include "Agent.h"

#include <limits>

#include <BWAPI.h>

using namespace BWAPI;


bool ResourceAdvisor::makeAgentGatherMinerals(Agent& agent)
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

Unit* ResourceAdvisor::getClosestMineralPatch(const Agent& agent)
{
	Unit *closest = NULL;
	int   minDist = std::numeric_limits<int>::max();

	// TODO: change this so that ResourceAdvisor tracks discovered resource fields
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

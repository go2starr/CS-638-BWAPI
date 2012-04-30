/*
 *  ResourceAdvisor.cpp  
 *  
 *  This advisor provides info regarding map resources
 */
#include "ResourceAdvisor.h"
#include "Common.h"
#include "Agent.h"

#include <limits>

#include <BWAPI.h>

using namespace BWAPI;


UnitSet ResourceAdvisor::minerals;
UnitSet ResourceAdvisor::geysers;


void ResourceAdvisor::discoverMineralPatch( Unit& mineral )
{
	if (!mineral.getType().isMineralField()) {
		Broodwar->sendText("WARNING: Non-mineral field inserted into minerals!!");
		return;
	}
	minerals.insert(&mineral);
}

void ResourceAdvisor::discoverVespeneGeyser( Unit& geyser )
{
	// TODO: should probably verify unit type
	geysers.insert(&geyser);
}

bool ResourceAdvisor::makeAgentGatherMinerals( Agent& agent )
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

Unit* ResourceAdvisor::getClosestMineralPatch( const Agent& agent )
{
	Unit *closest = NULL;
	int   minDist = std::numeric_limits<int>::max();

	UnitSetIter it  = minerals.begin();
	UnitSetIter end = minerals.end();
	for(; it != end; ++it)
	{
		Unit *unit = *it;
		
		// Ignore used up patches
		if( unit->getResources() <= 0 )
			continue;

		// Used up patches become "Unknown" type
		if (!unit->getType().isMineralField() || unit->getType() == UnitTypes::Unknown)
			continue;

		int dist = agent.getUnit().getDistance(unit);
		if (dist < minDist)
		{
			minDist = dist;
			closest = unit;
		}
	}

	return closest;
}

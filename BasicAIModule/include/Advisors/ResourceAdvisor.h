#pragma once
/*
 *  ResourceAdvisor.h  
 *  
 *  This advisor provides info regarding map resources
 */
#include "Agent.h"

namespace BWAPI { class Unit; }


class ResourceAdvisor
{
public:
	static bool makeAgentGatherMinerals(Agent& agent);
	static BWAPI::Unit* getClosestMineralPatch(const Agent& agent);
};

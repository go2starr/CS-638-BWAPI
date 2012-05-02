#pragma once
/*
 *  ResourceAdvisor.h  
 *  
 *  This advisor provides info regarding map resources
 */
#include "Common.h"

namespace BWAPI { class Unit; }


class ResourceAdvisor
{
private:
	static UnitSet minerals;
	static UnitSet geysers;

public:
	static void discoverMineralPatch(BWAPI::Unit& mineral);
	static void discoverVespeneGeyser(BWAPI::Unit& geyser);

	static BWAPI::Unit* getClosestMineralPatch(const Agent& agent);
};

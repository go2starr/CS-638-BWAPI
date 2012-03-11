/*
 * SCVAgent.cpp
 */
#include "UnitAgents/SCVAgent.h"
#include "GroundAgent.h"

#include <BWAPI.h>

using BWAPI::Unit;


SCVAgent::SCVAgent(Unit &u) 
	: GroundAgent(u)
{}

void SCVAgent::update()
{
	if (state) {
		switch (state) {
			case GatherState:
				// Gather minerals
				if (unitTarget->getType().isMineralField()) 
				{
					if (!unit.isGatheringMinerals())
						unit.gather(unitTarget);
				// Gather gas
				} else if (unitTarget->getType().isRefinery()) {
					if (!unit.isGatheringGas())
						unit.gather(unitTarget);
				}
				break;

			case BuildState:
				if (!unit.isConstructing())
				{
					//BWAPI::UnitTypes::Terran_Supply_Depot.tileHeight;
					//unit.build(unitTypeTarget);
				}

			default:
				GroundAgent::update();
		}
	}
}

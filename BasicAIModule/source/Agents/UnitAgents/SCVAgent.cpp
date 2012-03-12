/*
 * SCVAgent.cpp
 */
#include "UnitAgents/SCVAgent.h"
#include "GroundAgent.h"
#include <TacticalBuildingPlacer.h>

#include <BWAPI.h>

using BWAPI::Unit;
using BWAPI::Broodwar;


SCVAgent::SCVAgent(Unit &u) 
	: GroundAgent(u)
{}

void SCVAgent::update()
{
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
				if (!buildingReserved)
				{
					Broodwar->sendText("%d:: Reserving space for: %s", unit.getID(), unitTypeTarget.getName().c_str());
					buildingLocation = TacticalBuildingPlacer::instance().reserveBuildLocation(unitTypeTarget, 
																								BWAPI::Broodwar->self()->getStartLocation(), 
																								&unit);
					if (buildingLocation != BWAPI::TilePositions::None)
					{
						// TODO: Need to reset this to false eventually
						buildingReserved = true;
					}
				}
				unit.build(buildingLocation, unitTypeTarget);
			}
	}

	GroundAgent::update();
}

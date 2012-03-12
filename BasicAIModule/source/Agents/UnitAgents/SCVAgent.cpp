/*
 * SCVAgent.cpp
 */
#include "UnitAgents/SCVAgent.h"
#include "GroundAgent.h"
#include <TacticalBuildingPlacer.h>

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
				}

				// Gather gas
				else if (unitTarget->getType().isRefinery()) 
				{
					if (!unit.isGatheringGas())
						unit.gather(unitTarget);
				}
				break;

			case BuildState:

				// Return cargo
				if (unit.isCarryingGas() ||
					unit.isCarryingMinerals())
				{
					unit.returnCargo();
				}

				// Reserve a build location
				else if (!buildingReserved)
				{
					TacticalBuildingPlacer &tbp = TacticalBuildingPlacer::instance();
					buildingLocation = tbp.reserveBuildLocation(unitTypeTarget, 
																BWAPI::Broodwar->self()->getStartLocation(), 
																&unit);
						if (buildingLocation != BWAPI::TilePositions::None)
						{
							// TODO: Need to reset this to false eventually
							// TODO: Pass off/cancel reservations on build fails
							buildingReserved = true;
						}
				}

				// Start building
				else if (!unit.isConstructing())
				{
					unit.build(buildingLocation, unitTypeTarget);
				}

				break;
			default:
				GroundAgent::update();
		
		}
	}
}
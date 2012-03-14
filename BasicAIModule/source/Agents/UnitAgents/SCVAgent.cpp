/*
 * SCVAgent.cpp
 */
#include "SCVAgent.h"
#include "GroundAgent.h"

#include "TacticalBuildingPlacer.h"

#include <BWAPI.h>

using namespace BWAPI;


SCVAgent::SCVAgent(Unit &u) 
	: GroundAgent(u)
{
	wasConstructing = false;
}

void SCVAgent::update()
{
	switch (state) {

		case IdleState:
			// be idle
			break;
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
		// don't do this in the build state, we are here to build only
//			if (unit.isCarryingGas() ||	unit.isCarryingMinerals()) {
//				unit.returnCargo();
//			}
			// go to idle state after finishing a building, so you can be re assigned
			// otherwise worker will keep finding new loactions and keep building
			if (!unit.isConstructing() && wasConstructing)
			{
				wasConstructing = false;
				// go to default state, for now idle
				setState(IdleState);
			}
			// Reserve a build location
			else if (!unit.isConstructing() && !buildingReserved)
			{
				// find build location for target, TacticalBuildingPlacer decides what is best
				// needs better seed location, don't always pick the same one
				buildingLocation = TacticalBuildingPlacer::instance().reserveBuildLocation(unitTypeTarget, Broodwar->self()->getStartLocation(), &unit );

				if (buildingLocation != TilePositions::None)
				{
					// TODO: Need to reset this to false eventually
					// TODO: Pass off/cancel reservations on build fails
					buildingReserved = true;
					// save our build location
					buildLocation = buildingLocation;
					
				}
			}
			else if (!unit.isConstructing() && buildingReserved) {
				// build will return false if not enough money
				// keep trying to build each update
				unit.build(buildingLocation, unitTypeTarget);
			}
			// unit.build returned true
			// once starting construction, don't worry about the build loacation
			else if (unit.isConstructing() && buildingReserved) {
				buildingReserved = false;
				wasConstructing = true;
			}

			break;
	}

    GroundAgent::update();
}
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
    , wasConstructing(false)
    , constructingStructure(NULL)
{ }

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
			//if (unit.isCarryingGas() ||	unit.isCarryingMinerals()) {
			//	unit.returnCargo();
			//}

			// Done?
			if (!unit.isConstructing() && constructingStructure != NULL)
			{
				if (constructingStructure->isCompleted())
				{
					constructingStructure = NULL;
					buildingReserved = false;
					state = IdleState;
				}
			}

			// Reserve a build location
			else if (!buildingReserved)
			{
				TacticalBuildingPlacer &tbp = TacticalBuildingPlacer::instance();
				buildingLocation = tbp.instance().reserveBuildLocation(unitTypeTarget, Broodwar->self()->getStartLocation(), &unit);
				if (buildingLocation != TilePositions::None)
				{
					buildingReserved = true;
				}
				unit.move(Position(buildingLocation));
			}

			// Build it
			else if (buildingReserved && !unit.isConstructing())
			{
				unit.build(buildingLocation, unitTypeTarget);
			}

			// Building...
			else if (unit.isConstructing())
			{
				constructingStructure = unit.getBuildUnit();
			}
			break;
	}

    GroundAgent::update();
}
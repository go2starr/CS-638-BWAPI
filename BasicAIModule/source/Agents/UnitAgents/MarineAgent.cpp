/*
 * MarineAgent.cpp
 */
#include "MarineAgent.h"
#include "GroundAgent.h"

#include <BWAPI.h>

using namespace BWAPI;


MarineAgent::MarineAgent(Unit& u)
	: GroundAgent(u)
{

}

void MarineAgent::update()
{
//    state = DefendState;
	switch (state) {
		case DefendState:
		case IdleState:
			// Load into bunkers
			UnitSet unitsInRange = unit.getUnitsInRadius(200);
			for (UnitSetIter it = unitsInRange.begin(); it != unitsInRange.end(); it++) {
				if ((*it)->getType() == UnitTypes::Terran_Bunker &&
					(*it)->getLoadedUnits().size() < 4)
						(*it)->load(&unit);
			}
	}

    GroundAgent::update();
}



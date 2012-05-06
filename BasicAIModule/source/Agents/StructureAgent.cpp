#include <StructureAgent.h>
#include <MapAdvisor.h>
#include <BWTA.h>

#include <cmath>

using namespace BWAPI;

// currenty is set up to produce / train agents
// this should be moved to production agent or something
// which would also inherit from structure agent
void StructureAgent::update()
{
	// Send Units to nearest chokepoint
	Position rallyPoint = MapAdvisor::getPositionOutsideNearestChokepoint(unit.getPosition());
	if (rallyPoint != NULL)
		unit.setRallyPoint(rallyPoint);

	switch (state)
	{
	case IdleState:
		break;

	case TrainState:
	case BuildState:
		// Need new target?
		if (unitTypeTarget == UnitTypes::None &&
			!unit.isTraining() &&
			!unit.isBeingConstructed() &&
			!unit.isConstructing()) {
			if (!buildQueue.empty()) {
				unitTypeTarget = buildQueue.front();
				buildQueue.pop();
			} else {
				setState(IdleState);
			}
		}

		// Build Addon?
		if (unitTypeTarget.isAddon()) {
			unit.buildAddon(unitTypeTarget);

			// Done?
			if (unit.getAddon() != NULL && 
				unit.getAddon()->getType().getID() == unitTypeTarget.getID()) {
				setUnitTypeTarget(UnitTypes::None);
			}
		}
		// Else, build units
		else {
			if (!unit.isTraining() && !unit.isBeingConstructed()) 
			{
				unit.train(unitTypeTarget);
			}
			else
			{
				setUnitTypeTarget(UnitTypes::None);
			}
			break;
		}
	}
}
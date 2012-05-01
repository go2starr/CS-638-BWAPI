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
		// fall through

	case TrainState:
		if (!unit.isTraining() && !unit.isBeingConstructed()) 
		{
			unit.train(unitTypeTarget);
		}
		else
		{
			//state = IdleState;
		}
		break;

	case BuildState:
		if (unitTypeTarget.isAddon())
		{
			unit.buildAddon(unitTypeTarget);
			if (unit.getAddon() != NULL && 
				unit.getAddon()->getType().getID() == unitTypeTarget.getID())
				state = IdleState;
		}
	}
}
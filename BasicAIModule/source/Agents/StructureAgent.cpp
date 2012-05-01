#include <StructureAgent.h>
#include <BWTA.h>

#include <cmath>

using namespace BWAPI;

// currenty is set up to produce / train agents
// this should be moved to production agent or something
// which would also inherit from structure agent
void StructureAgent::update()
{
	// Send Units to nearest chokepoint
    BWTA::Chokepoint* chokepoint = BWTA::getNearestChokepoint(unit.getPosition());
	if( chokepoint != NULL ) {
		Position p = chokepoint->getCenter();
		int px = p.x();
		int py = p.y();

		// Get a rough estimate of unit vector from Unit to Chokepoint
		int dx = px - unit.getPosition().x();
		int dy = py - unit.getPosition().y();
		int d  = dx + dy;  // close enough

		// Offset outward
		px += dx * 50 / d;
		py += dy * 50 / d;
	}

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
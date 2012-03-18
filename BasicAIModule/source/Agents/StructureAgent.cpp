#include <StructureAgent.h>
#include <BWTA.h>

void StructureAgent::update()
{
	// Send Units to nearest chokepoint
    BWTA::Chokepoint* chokepoint = BWTA::getNearestChokepoint(unit.getPosition());
    if( chokepoint != NULL )
        unit.setRallyPoint(chokepoint->getCenter());

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
			state = IdleState;
		}
		break;

		case BuildState:
		if (unitTypeTarget.isAddon())
		{
			if (unit.getAddon()->getType().getID() != unitTypeTarget.getID())
				unit.buildAddon(unitTypeTarget);
			else
				state = IdleState;
		}
	}
}
#include <StructureAgent.h>

void StructureAgent::update()
{
	switch (state)
	{
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
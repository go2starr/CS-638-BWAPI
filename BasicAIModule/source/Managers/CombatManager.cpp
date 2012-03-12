#include "CombatManager.h"
#include <BWAPI.h>

using namespace BWAPI;
using namespace std;

void CombatManager::update()
{
	Broodwar->drawTextScreen(2, 20, "\x11 CM : (SCV=%d) (Marine=%d)", 
		numAgents(UnitTypes::Terran_SCV), 
		numAgents(UnitTypes::Terran_Marine));

	/* Produce an army */
	for (set<Agent*>::iterator it = agents.begin(); it != agents.end(); it++)
	{
		Agent *agent = *it;
		UnitType ut = agent->getUnit().getType();

		/* Tell SCVs to build a barracks */
		if (ut.isWorker())
		{
			agent->setState(BuildState);
			agent->setUnitTypeTarget(UnitTypes::Terran_Barracks);
		}

		/* Tell barracks to build marines */
		else if (ut == UnitTypes::Terran_Barracks
             && !agent->getUnit().isBeingConstructed())
		{
			agent->setState(TrainState);
			agent->setUnitTypeTarget(UnitTypes::Terran_Marine);
		}
	}
	
	/* Base class updates Agents */
	Manager::update();
}
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
		BWAPI::UnitType ut = agent->getUnit().getType();

		/* Tell SCVs to build a barracks */
		if (ut.isWorker())
		{
			agent->setState(BuildState);
			agent->setUnitTypeTarget(BWAPI::UnitTypes::Terran_Barracks);
		}

		/* Tell barracks to build marines */
		else if (ut.getID() == BWAPI::UnitTypes::Terran_Barracks.getID())
		{
			agent->setState(TrainState);
			agent->setUnitTypeTarget(BWAPI::UnitTypes::Terran_Marine);
		}
	}
	
	/* Base class updates Agents */
	Manager::update();
}
#include "CombatManager.h"
#include <BWAPI.h>

using namespace BWAPI;
using namespace std;

void CombatManager::update()
{
	Broodwar->drawTextScreen(2, 20, 
        "\x11 CM : (SCV=%d) (Marine=%d) (Firebat=%d) (Medic=%d)", 
		numAgents(UnitTypes::Terran_SCV), 
		numAgents(UnitTypes::Terran_Marine), 
        numAgents(UnitTypes::Terran_Firebat), 
        numAgents(UnitTypes::Terran_Medic));

	/* Produce an army */
	for (set<Agent*>::iterator it = agents.begin(); it != agents.end(); it++)
	{
		Agent *agent = *it;
		UnitType ut = agent->getUnit().getType();

		/* Tell SCVs to build a barracks */
		// Note: numAgents will only for what is completed,
		// doesn't include what is being constructed already
		if (ut.isWorker() && numAgents(UnitTypes::Terran_Barracks) < 2)
		{
			agent->setState(BuildState);
			agent->setUnitTypeTarget(UnitTypes::Terran_Barracks);
		}

		/* Tell barracks to build marines and firebats*/
		else if (ut == UnitTypes::Terran_Barracks
             && !agent->getUnit().isBeingConstructed())
		{
            static int counter = 0;
            if( ++counter % 4 == 0 ) // every 4th unit is a medic
            {   
                if (Broodwar->self()->gas() > 25)
                {
                    agent->setState(TrainState);
                    agent->setUnitTypeTarget(UnitTypes::Terran_Medic);
                } else { 
                    Broodwar->drawTextMap(
                        agent->getUnit().getPosition().x(), agent->getUnit().getPosition().y() + 10, 
                        "Can't train Medic, need gas..." );
                }
            } else { 
			    agent->setState(TrainState);
			    agent->setUnitTypeTarget(UnitTypes::Terran_Marine);
            }
		}
	}
	
	/* Base class updates Agents */
	Manager::update();
}
#include "CombatManager.h"
#include <BWAPI.h>
#include <BWTA.h>

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

		/* Tell barracks to build marines, firebats, and medics */
		else if (ut == UnitTypes::Terran_Barracks
             && !agent->getUnit().isBeingConstructed())
		{
            /* TODO : add this back in (or something like it) when we can build an Academy
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
            */
			    agent->setState(TrainState);
			    agent->setUnitTypeTarget(UnitTypes::Terran_Marine);
            //}
		}
	}

    // Setup a relatively small enemy base assault if we have a decent number of marines
    // TODO - CombatManager should start putting it's Agents in Squads
    // then when a Squad is full, and the CombatManager gets assigned the 
    // 'Attack' task, it can send whole squads at a time 
    // We could track two more sets of Agent*, 
    // one would be currently unassigned Agent's, the other would be assigned
    const int numMarines = numAgents(UnitTypes::Terran_Marine);
    const int threshold = 16;
    if( numMarines >= threshold )
    {
        Player* enemy = Broodwar->enemy();
        if( enemy != NULL )
        {
            // Get the likely enemy base location (furthest from myStart)
            // TODO - this should probably be calculated only once, 
            // maybe we should have a Manager::onMatchStart() method?
            TilePosition myStart = Broodwar->self()->getStartLocation();
            TilePosition target;
            double maxDistance = 0.0;
            set<TilePosition>& startPositions = Broodwar->getStartLocations();
            set<TilePosition>::iterator pit  = startPositions.begin();
            set<TilePosition>::iterator pend = startPositions.end();
            for(; pit != pend; ++pit)
            {
                TilePosition pos = *pit;
                const double distance = pos.getDistance(myStart);
                if( distance > maxDistance )
                {
                    target = pos;
                    maxDistance = distance;
                }
            }

            // If a valid target was found
            if( target.isValid() )
            {
                // Setup the attack force:
                // set agents states to Attack until 
                // - we reach the number we want, or
                // - we run out of marines to assign
                const Position enemyBase(target);
                const int attackNum = numMarines / 2;
                AgentSet marines(getAgentsOfType(UnitTypes::Terran_Marine));
                AgentSetIter it  = marines.begin();
                AgentSetIter end = marines.end();
                for(int i = 0; i < attackNum && it != end; ++i, ++it)
                {
                    Agent* marine = *it;
                    marine->setState(AttackState);
                    marine->setPositionTarget(enemyBase);
                }
            } else { // complain about it
                if( Broodwar->getFrameCount() % 60 == 0 )
                    Broodwar->sendText("CombatMgr: attack target is invalid");
            }
        }
    }
	
	/* Base class updates Agents */
	Manager::update();
}

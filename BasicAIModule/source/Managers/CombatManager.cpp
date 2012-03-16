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

    // Setup a relatively small enemy base assault if we have a decent number of marines
    // TODO - CombatManager should start putting it's Agents in Squads
    // then when a Squad is full, and the CombatManager gets assigned the 
    // 'Attack' task, it can send whole squads at a time 
    // We could track two more sets of Agent*, 
    // one would be currently unassigned Agent's, the other would be assigned
    const int numMarines = numAgents(UnitTypes::Terran_Marine);
    const int threshold = 50;
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

				// TODO INCLUDE ALL UNIT TYPES
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

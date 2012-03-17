#include "CombatManager.h"
#include "Common.h"
#include "Squad.h"

#include <BWAPI.h>
#include <BWTA.h>

#include <string>

using namespace BWAPI;
using BWTA::Chokepoint;

using namespace std;


void CombatManager::onMatchStart()
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

        enemyBase = Position(target);
    }
}

void CombatManager::update()
{
	Broodwar->drawTextScreen(2, 20, 
        "\x11 CM : (SCV=%d) (Marine=%d) (Firebat=%d) (Medic=%d)", 
		numAgents(UnitTypes::Terran_SCV), 
		numAgents(UnitTypes::Terran_Marine), 
        numAgents(UnitTypes::Terran_Firebat), 
        numAgents(UnitTypes::Terran_Medic));

    // If we have any new Agents, they should go in the unassigned set
    AgentSetIter it  = agents.begin();
    AgentSetIter end = agents.end();
    for(; it != end; ++it)
    {
        Agent *agent = *it;
        if( assignedAgents.find(agent) == assignedAgents.end() ) 
        {
            unassignedAgents.insert(agent);
            // TODO - this is a hack to make them defend in the right place
            Chokepoint *cp = BWTA::getNearestChokepoint(agent->getUnit().getPosition());
            if( cp != NULL )
                agent->setPositionTarget(cp->getCenter());
            else 
                agent->setPositionTarget(Position(Broodwar->self()->getStartLocation()));
        }
    }
        

    // Setup a relatively small enemy base assault if we have a decent number of marines
    // TODO - CombatManager should start putting it's Agents in Squads
    // then when a Squad is full, and the CombatManager gets assigned the 
    // 'Attack' task, it can send whole squads at a time 
    // We could track two more sets of Agent*, 
    // one would be currently unassigned Agent's, the other would be assigned
    const int numMarines = numAgents(UnitTypes::Terran_Marine);
    const int numFirebats = numAgents(UnitTypes::Terran_Firebat);
    const int numMedics = numAgents(UnitTypes::Terran_Medic);
    const int numTroops = numMarines + numFirebats + numMedics;

    const int threshold = 40;
    if( numTroops >= threshold )
    {
        // Setup the attack force:
        // set agents states to Attack until 
        // - we reach the number we want, or
        // - we run out of marines to assign
        const int attackNum = numTroops / 2;

        // TODO : add units to a squad and put them in the assigned set

        int numAssignedTroops = 0;

        AgentSet marines(getAgentsOfType(UnitTypes::Terran_Marine, unassignedAgents));
        AgentSetIter it  = marines.begin();
        AgentSetIter end = marines.end();
        for(; it != end && numAssignedTroops < attackNum; 
            ++it, ++numAssignedTroops)
        {
            Agent* marine = *it;
            marine->setState(AttackState);
            marine->setPositionTarget(enemyBase);
            
            unassignedAgents.erase(marine);
            assignedAgents.insert(marine);
        }

        AgentSet firebats(getAgentsOfType(UnitTypes::Terran_Firebat, unassignedAgents));
        it  = firebats.begin();
        end = firebats.end();
        for(; it != end && numAssignedTroops < attackNum; 
            ++it, ++numAssignedTroops)
        {
            Agent* firebat = *it;
            firebat->setState(AttackState);
            firebat->setPositionTarget(enemyBase);

            unassignedAgents.erase(firebat);
            assignedAgents.insert(firebat);
        }

        AgentSet medics(getAgentsOfType(UnitTypes::Terran_Medic, unassignedAgents));
        it  = medics.begin();
        end = medics.end();
        for(; it != end && numAssignedTroops < attackNum; 
            ++it, ++numAssignedTroops)
        {
            Agent* medic = *it;
            medic->setState(AttackState);
            medic->setPositionTarget(enemyBase);

            unassignedAgents.erase(medic);
            assignedAgents.insert(medic);
        }
    }
	
	/* Base class updates Agents */
	Manager::update();
}

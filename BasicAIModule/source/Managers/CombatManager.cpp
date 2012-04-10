#include "CombatManager.h"
#include "SquadAdvisor.h"
#include "Common.h"
#include "Squad.h"

#include <BWAPI.h>
#include <BWTA.h>

#include <algorithm>
#include <string>

using namespace BWAPI;
using BWTA::Chokepoint;

using namespace std;


void CombatManager::onMatchStart()
{
    Player* enemy = Broodwar->enemy();
    if( enemy != NULL )
    {
        enemyBase = Position(SquadAdvisor::getFarthestEnemyStartLocation(Broodwar->self()->getStartLocation()));
    }
}

void CombatManager::onMatchEnd(bool isWinner)
{
    for_each(squads.begin(), squads.end(), DeleteObjectFunctor());
    squads.clear();
}

void CombatManager::update()
{
	// Get new agents into state
	addNewAgents();  

	// TODO : Merge squads

	// Attack?
	const int numTroops = agents.size();
    const int threshold = 70;

    if( numTroops >= threshold )
    {
        // Attack with full force (adjust to lower)
		for (SquadVectorIter it = squads.begin(); it != squads.end(); it++)
		{
			Agent *a = (*it)->getLeader();
			if (a != NULL)
			{
				a->setState(AttackState);
				a->setPositionTarget(enemyBase);
			}
		}
	}

	/* Update squads */
	for (SquadVectorIter it = squads.begin(); it != squads.end(); it++)
	{
		(*it)->update();
	}
	/* Base class updates Agents */
	Manager::update();
}


void CombatManager::addNewAgents()
{
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

			// Assign Agent to a Squad (only 1 atm)
			if (squads.empty())
			{
				squads.push_back(new Squad("test-squad"));
			}
			// Create a new squad if current is full
			if (squads.back()->getAgents().size() > 10)
			{
				squads.push_back(new Squad("test-squad"));
			}
			Squad *squad = squads.back();

			squad->addAgent(*it);
			if (squad->getLeader() == NULL)
			{
				squad->setLeader(*it);
			}
	        unassignedAgents.erase(agent);
	        assignedAgents.insert(agent);
		}
	}
}

void CombatManager::draw()
{
	Broodwar->drawTextScreen(2, 20, 
        "\x11 CM : (Enemies=%d) : (Marine=%d) (Firebat=%d) (Medic=%d)", 
	    enemyUnits.size(), 
	    numAgents(UnitTypes::Terran_Marine), 
        numAgents(UnitTypes::Terran_Firebat), 
        numAgents(UnitTypes::Terran_Medic));

	for (SquadVectorIter it = squads.begin(); it != squads.end(); it++)
	{
		(*it)->draw();
	}

	Manager::draw();
}

void CombatManager::discoverEnemyUnit(Unit* unit)
{
    enemyUnits.insert(unit);
}

Unit* CombatManager::findEnemyUnit(const UnitType& type)
{
    Unit *unit = NULL;

    UnitSetConstIter it  = enemyUnits.begin();
    UnitSetConstIter end = enemyUnits.end();
    for(; it != end; ++it)
    {
        if( *it == unit )
        {
            unit = *it;
            break;
        }
    }

    return unit;
}

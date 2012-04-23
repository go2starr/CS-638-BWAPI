#include "CombatManager.h"
#include "SquadAdvisor.h"
#include "Common.h"
#include "Squad.h"

#include <BWAPI.h>
#include <BWTA.h>

#include <algorithm>
#include <string>
#include <limits>

using namespace BWAPI;
using BWTA::Chokepoint;

using namespace std;


void CombatManager::onMatchStart()
{
    Player* enemy = Broodwar->enemy();
    if( enemy != NULL )
    {
        const TilePosition& myBase(Broodwar->self()->getStartLocation());
        enemyBase = Position(SquadAdvisor::getFarthestEnemyBase(myBase));
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
	const int numTroops = numLivingAgents();
    const int threshold = 10;

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

    // Clean up squads where everyone is dead 
    vector<SquadVectorIter> itersToErase;
    for(SquadVectorIter it = squads.begin(); it != squads.end(); ++it)
    {
        Squad *squad = *it;
        if( squad->numAlive() == 0 )
        {
            delete squad;
            itersToErase.push_back(it);
        }
    }
    for(vector<SquadVectorIter>::iterator it = itersToErase.begin();
        it != itersToErase.end(); ++it)
    {
        squads.erase(*it);
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
            // TODO: if current squad has filled its requirements...
			if (squads.back()->getAgents().size() > 10)
			{
				squads.push_back(new Squad("test-squad"));
			}
			Squad *squad = squads.back();

            // TODO: only add the agent if we have (or can create)
            // a squad that has an unfulfilled requirement for this type of unit,
            // otherwise leave it in the unassigned set
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
	SquadVectorConstIter it  = squads.begin();
	SquadVectorConstIter end = squads.end();
	for(; it != end; ++it)
	{
		(*it)->draw();
	}

	Manager::draw();
}

int CombatManager::numLivingAgents() const
{
    int num = 0;

    AgentSetConstIter it  = agents.begin();
    AgentSetConstIter end = agents.end();
    for(; it != end; ++it)
    {
        const Agent *agent = *it;
        if( agent->getUnit().getHitPoints() > 0 ) 
            ++num;
    }

    return num;
}

void CombatManager::discoverEnemyUnit(Unit* unit)
{
    enemyUnits.insert(unit);
    if( unit->getType().isBuilding() )
    {
        enemyBuildings.insert(unit);
    }
    else
    {
        enemyActors.insert(unit);
    }
}

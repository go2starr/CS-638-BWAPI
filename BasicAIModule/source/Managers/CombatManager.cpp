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
	// Get new agents into state
	addNewAgents();        

	// Attack?
	const int numTroops = agents.size();
    const int threshold = 50;

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
				squads.push_back(new Squad());
			}
			Squad *squad = squads.at(0);

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
       "\x11 CM : (SCV=%d) (Marine=%d) (Firebat=%d) (Medic=%d)", 
	numAgents(UnitTypes::Terran_SCV), 
	numAgents(UnitTypes::Terran_Marine), 
    numAgents(UnitTypes::Terran_Firebat), 
    numAgents(UnitTypes::Terran_Medic));

	Manager::draw();
}
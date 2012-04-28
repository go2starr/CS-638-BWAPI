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

// how large to make attack squads
// and when to attack with them
static int AttackSquadSize = 10;

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
	for_each(attackSquads.begin(), attackSquads.end(), DeleteObjectFunctor());
	attackSquads.clear();
	for_each(defendSquads.begin(), defendSquads.end(), DeleteObjectFunctor());
	defendSquads.clear();
	for_each(bunkerSquads.begin(), bunkerSquads.end(), DeleteObjectFunctor());
	bunkerSquads.clear();
}

void CombatManager::update()
{
	// Get new agents into state
	addNewAgents();  

	// TODO : Merge squads

	// Attack?
	const int numTroops = numLivingAgents();
	//const int threshold = 15;


	// Attack with full squad force (adjust to lower)
	for (SquadVectorIter it = attackSquads.begin(); it != attackSquads.end(); it++)
	{
		if ((*it)->getSize() == AttackSquadSize)
		{
			Agent *a = (*it)->getLeader();
			if (a != NULL)
			{
				a->setState(AttackState);
				a->setPositionTarget(enemyBase);
			}
		}
	}



	// Move marines from defense squad to bunker squad if one available
	if ((int)bunkerAgents.size() > 0 
		&& (int)bunkerSquads.size() > 0 
		&& (int)defendSquads.size() > 0)
	{
		while ( bunkerSquads.back()->getSize() < 4 
			&& defendSquads.back()->getSize() > 0)
		{
			defendSquads.back()->moveAgent(*(defendSquads.back()->getAgents().begin()), bunkerSquads.back());
		}
	}

	/* Update attack, defend, and bunker squads */
	for (SquadVectorIter it = attackSquads.begin(); it != attackSquads.end(); it++)
	{
		(*it)->update();
	}
	for (SquadVectorIter it = defendSquads.begin(); it != defendSquads.end(); it++)
	{
		(*it)->update();
	}
	for (SquadVectorIter it = bunkerSquads.begin(); it != bunkerSquads.end(); it++)
	{
		(*it)->update();
	}

	// Clean up squads where everyone is dead 
	vector<SquadVectorIter> itersToErase;
	for(SquadVectorIter it = attackSquads.begin(); it != attackSquads.end(); ++it)
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
		attackSquads.erase(*it);
	}
	itersToErase.clear();
	for(SquadVectorIter it = defendSquads.begin(); it != defendSquads.end(); ++it)
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
		defendSquads.erase(*it);
	}
	itersToErase.clear();
	for(SquadVectorIter it = bunkerSquads.begin(); it != bunkerSquads.end(); ++it)
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
		bunkerSquads.erase(*it);
	}
	itersToErase.clear();


	/* Base class updates Agents */
	Manager::update();
}


void CombatManager::addNewAgents()
{
	// If we have any new Agents, they should go in the unassigned set
	AgentSetIter it  = agents.begin();
	AgentSetIter end = agents.end();
	Squad * squad;

	for(; it != end; ++it)
	{
		Agent *agent = *it;
		UnitType ut = agent->getUnit().getType();

		// if not assigned yet
		if( assignedAgents.find(agent) == assignedAgents.end() ) 
		{

			// check for bunkers
			if (ut == UnitTypes::Terran_Bunker) {
				bunkerAgents.insert(agent);
				assignedAgents.insert(agent);
				continue;
			}
			// init to unassigned
			unassignedAgents.insert(agent);

			// TODO - this is a hack to make them defend in the right place
			Chokepoint *cp = BWTA::getNearestChokepoint(agent->getUnit().getPosition());
			if( cp != NULL )
				agent->setPositionTarget(cp->getCenter());
			else 
				agent->setPositionTarget(Position(Broodwar->self()->getStartLocation()));

			// Assign Agent to a Squad

			// if there isn't an empty bunker fill up defense squad
			// with marines at max of 4 (only 1 bunker atm)
			// these will be reassigned to a bunker squad eventually
			if ((int)bunkerAgents.size() == 0 
				&& ut == UnitTypes::Terran_Marine 
				&& ( (int)defendSquads.size() == 0 || defendSquads.back()->getSize() < 4)) 
			{
				// if there isn't a defend squad make one
				if ((int)defendSquads.size() == 0)
				{
					defendSquads.push_back(new Squad("defend-squad", defend));
				}
				// set pointer
				squad = defendSquads.back();
			}
			else if ((int)bunkerAgents.size() == 1 
				&& ut == UnitTypes::Terran_Marine
				&& ((int)bunkerSquads.size() == 0 || bunkerSquads.back()->getSize() < 4))
			{
				// if there isn't a bunker squad make one
				if ((int)bunkerSquads.size() == 0)
				{
					bunkerSquads.push_back(new Squad("bunker-squad", bunker));
					// always assigns first bunker as target
					// TODO: make better bunker agent arrangement
					bunkerSquads.back()->setBunkerTarget(&(*bunkerAgents.begin())->getUnit());
				}
				// set pointer
				squad = bunkerSquads.back();
			}
			else 
			{
				// if there isn't an attack squad make one
				if ((int)attackSquads.size() == 0 
					|| attackSquads.back()->getSize() == AttackSquadSize)
				{
					attackSquads.push_back(new Squad("attack-squad", attack));
					// increase squad strength for each new squad
					AttackSquadSize += 5;
				}
				squad = attackSquads.back();
			}

			// TODO: only add the agent if we have (or can create)
			// a squad that has an unfulfilled requirement for this type of unit,
			// otherwise leave it in the unassigned set
			squad->addAgent(*it);
			if (squad->getLeader() == NULL)
			{
				squad->setLeader(*it);
			}

			// change to assigned
			unassignedAgents.erase(agent);
			assignedAgents.insert(agent);
		}
	}
}

void CombatManager::draw()
{
	SquadVectorConstIter it, end;

	it  = attackSquads.begin();
	end = attackSquads.end();
	for(; it != end; ++it)
	{
		(*it)->draw();
	}
	it  = defendSquads.begin();
	end = defendSquads.end();
	for(; it != end; ++it)
	{
		(*it)->draw();
	}
	it  = bunkerSquads.begin();
	end = bunkerSquads.end();
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

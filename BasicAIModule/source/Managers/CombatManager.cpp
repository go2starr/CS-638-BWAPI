#include "CombatManager.h"
#include "SquadAdvisor.h"
#include <MapAdvisor.h>
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
	for_each(attackSquads.begin(), attackSquads.end(), DeleteObjectFunctor());
	attackSquads.clear();
	for_each(defendSquads.begin(), defendSquads.end(), DeleteObjectFunctor());
	defendSquads.clear();
}

void CombatManager::update()
{
	// Get new agents into state
	addNewAgents();  

	// Update leaders
	updateSquadLeaders();

	/* Update attack, defend, and bunker squads */
	for (SquadVectorIter it = attackSquads.begin(); it != attackSquads.end(); it++)
	{
		(*it)->update();
	}
	for (SquadVectorIter it = defendSquads.begin(); it != defendSquads.end(); it++)
	{
		(*it)->update();
	}

	// Remove squads without agents
	cleanUpSquads();

	/* Base class updates Agents */
	Manager::update();
}


void CombatManager::addNewAgents()
{
	for (AgentSetIter it = agents.begin(); it != agents.end(); ++it)
	{
		Squad *squad = NULL;
		Agent *agent = *it;
		UnitType ut = agent->getUnit().getType();

		// Agent does not have a squad?
		if(agentSquadMap.find(agent) == agentSquadMap.end()) 
		{
			// TODO - this is a hack to make them defend in the right place
			Position unitPosition = agent->getUnit().getPosition();
			Position rallyPoint = MapAdvisor::getPositionOutsideNearestChokepoint(unitPosition);
			if (rallyPoint != Positions::None)
				agent->setPositionTarget(rallyPoint);

			// Assign Agent to a Squad
			// Defense squad?
			if (defendSquads.size() < getDefendSquadTargetSize()) {
				if (!defendSquads.size() ||
					defendSquads.back()->getSize() >= DefendSquadSize)
					defendSquads.push_back(new Squad("defend-squad", defend));
				squad = defendSquads.back();
			}

			// Attack squad?
			else if (attackSquads.size() < getAttackSquadTargetSize()) {
				if (!attackSquads.size() ||
					attackSquads.back()->getSize() >= AttackSquadSize) 
					attackSquads.push_back(new Squad("attack-squad", attack));
				squad = attackSquads.back();
			}

			// Add
			squad->addAgent(agent);
			agentSquadMap[agent] = squad;

			// Set leader - TODO: This should really be constructor.  If omitted, we get
			// null pointers
			if (squad->getLeader() == NULL)
				squad->setLeader(agent);
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




void CombatManager::cleanUpSquads()
{
	// Clean up squads where everyone is dead 
	vector<SquadVectorIter> itersToErase;
	for(SquadVectorIter it = attackSquads.begin(); it != attackSquads.end(); ++it)
	{
		Squad *squad = *it;
		
		// Hack fix for crash
		if( squad == NULL ) continue;

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
		
		// Hack fix for crash
		if( squad == NULL ) continue;

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
}

unsigned CombatManager::getAttackSquadTargetSize() const
{
	return 5;
}

unsigned CombatManager::getDefendSquadTargetSize() const
{
	return 3;
}

void CombatManager::updateSquadLeaders()
{
	const int numTroops = numLivingAgents();

	// Update squad leaders
	for (SquadVectorIter it = attackSquads.begin(); it != attackSquads.end(); it++)
	{
		Agent *leader = (*it)->getLeader();
		BWAPI::Unit *leaderUnit = &(leader->getUnit());

		// Attack with full squad force (adjust to lower)
		// Otherwise while we are filling up, stand at chokepoint
		if ((*it)->getSize() == AttackSquadSize && leader != NULL && numTroops > 50)
		{
			leader->setState(AttackState);
			leader->setPositionTarget(enemyBase);
		}
		else if (leader != NULL){
			leader->setState(AttackState);
		}

		// hopefully this covers the back of the squad better (furthest from leader)
		int killZoneRadius = leaderUnit->getType().seekRange() + (*it)->getRadius();
		set<BWAPI::Unit *> unitsInRange = leaderUnit->getUnitsInRadius(killZoneRadius);
	
		if ((int)unitsInRange.size() > 0)
		{
			int hitPoints = 99999;
			BWAPI::Unit * enemyTargetInRange = NULL;

			// get weakest enemy or a medic target
			for (UnitSetIter unitIt = unitsInRange.begin(); unitIt != unitsInRange.end(); unitIt++)
			{
				// check for enemy
				if ((*unitIt)->getPlayer() != Broodwar->self() 
					&& ((*unitIt)->getType().canAttack() 
					|| (*unitIt)->getType() == BWAPI::UnitTypes::Terran_Medic
					|| (*unitIt)->getType() == BWAPI::UnitTypes::Terran_Bunker))
				{
					// also check shields
					int tempHitPoints = (*unitIt)->getHitPoints() + (*unitIt)->getShields();

					if (tempHitPoints == 0)
						continue;

					if ((*unitIt)->getType() == BWAPI::UnitTypes::Terran_Medic)
					{
						enemyTargetInRange = *unitIt;
						break;
					}
					else if (tempHitPoints < hitPoints)
					{
						hitPoints = tempHitPoints;
						enemyTargetInRange = *unitIt;
					}
				}
			}
			// find a target?
			if (enemyTargetInRange != NULL)
			{
				leader->setState(AttackState);
				leader->setUnitTarget(enemyTargetInRange);
			}
			else 
			{
				// reset if no targets in range
				leader->setUnitTarget(NULL);
			}
		}
	} // end squad leader update
}
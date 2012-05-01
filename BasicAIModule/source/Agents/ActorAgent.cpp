/* 
* ActorAgent.cpp
*/
#include "ActorAgent.h"
#include "Agent.h"
#include "Common.h"
#include "IncludeAllManagers.h"
#include "Strategizer.h"
#include "SquadAdvisor.h"
#include <BWAPI.h>
#include <BWTA.h>

using namespace BWTA;
using namespace BWAPI;

ActorAgent::ActorAgent(Unit &u)
: Agent(u)
{ }

void ActorAgent::update()
{
	switch(state)
	{
	case IdleState:
		break;

	case AttackState: 

		// check to make sure all targets are valid, ie on the map
		if (unitTarget != NULL && !unitTarget->getPosition().isValid())
			unitTarget = NULL;
		if (positionTarget != NULL && !positionTarget.isValid())
		{
			const TilePosition& myBase(Broodwar->self()->getStartLocation());
			positionTarget = Position(SquadAdvisor::getFarthestEnemyBase(myBase));
		}
			

		// always attack unit if there is a valid target that isn't already assigned
		// get position from unit target not from saved position target, this will be 
		// used if there isn't a unit target set
		//if (unitTarget != NULL && unit.getTarget() != unitTarget && unit.attack(unitTarget))
		if (unitTarget != NULL && !unit.isInWeaponRange(unitTarget))
		{
			// new unit target position?
			if (unit.getTargetPosition() != unitTarget->getPosition())
				unit.attack(unitTarget->getPosition());
		}
		else if (unitTarget != NULL)
		{
			// new unit target?
			if (unit.getTarget() != unitTarget)
				unit.attack(unitTarget);
		}
		// Move to where we want to attack (typically enemy base)
		// We will find some meaty targets along the ways
		//else if (!unit.isAttacking() && !unit.isMoving() && unit.getDistance(positionTarget) > 300)
		else if (positionTarget != NULL && unit.getDistance(positionTarget) > 300)
		{
			/*
			// Correct our destination if necessary
			Position tp = positionTarget;
			if (isOccupiedPosition(tp))
			{
			unit.attack(getClosestPosition(tp));
			positionTarget = unit.getTargetPosition();
			}
			*/

			// Move/attack
			// new target position?
			if (unit.getTargetPosition() != positionTarget)
				unit.attack(positionTarget);
		}

		break;

	case DefendState: 
		// always attack unit if there is a valid target that isn't already assigned
		/*if (unitTarget != NULL && unit.getTarget() != unitTarget && unit.attack(unitTarget))
		{}
		else if (!unit.isAttacking() && !unit.isMoving())
			unit.attack(positionTarget);*/ 
		break;

	case ScoutState:
		{
			/*
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

			//enemyBase = Position(target);
			*/
			//			BWAPI::TilePosition target = ScoutManager::getScoutTilePosition(unit.getTilePosition());
			//			ScoutManager scoutManager = Strategizer::scoutManager;
			//			BWAPI::TilePosition target = scoutManager.getScoutTilePosition(unit.getTilePosition());

			//			BWAPI::TilePosition target = Strategizer::instance()::ScoutManager::getScoutTilePosition(unit.getTilePosition());

			BWAPI::TilePosition target = Strategizer::instance().scoutManager.getScoutTilePosition(unit.getTilePosition(), unit.getType().isFlyer());

			//			Strategizer::instance().scoutManager.scratch();

			unit.move(Position(target));
		}
		break;
	}

	Agent::update();
}

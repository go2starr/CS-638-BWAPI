/* 
 * ActorAgent.cpp
 */
#include "ActorAgent.h"
#include "Agent.h"
#include "Common.h"
#include <BWAPI.h>
#include <BWTA.h>

using namespace BWTA;
using namespace BWAPI;

using std::pair;
using std::set;

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
	{
		// Move to attack target
		if (!unit.isAttacking() && !unit.isMoving() && unit.getDistance(positionTarget) > 300)
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
			unit.attack(positionTarget);
		}
	}
	break;

    case DefendState: 
		if (!unit.isAttacking() && !unit.isMoving())
			unit.attack(positionTarget); 
		break;

	case ScoutState:
		{
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
			unit.move(Position(target));
		}
		break;
	}

	Agent::update();
}


bool ActorAgent::isOccupiedPosition(Position target)
{
	UnitSet units = Broodwar->getUnitsInRadius(target, unit.getRight() - unit.getLeft());
	for (UnitSetIter it = units.begin(); it != units.end(); it++)
	{
		if ((*it)->getID() != unit.getID())
			return true;
	}
	return false;
}

/* BFS to find closest valid tile without Units */
Position ActorAgent::getClosestPosition(Position target)
{
	typedef pair<int,int> pt;
	queue<pt> open;
	map<pt, bool> visited;

	// Start at target
	open.push(pt(target.x(), target.y()));

	while (!open.empty())
	{
		// Visited?
		if (visited[open.front()])
		{
			open.pop();
			continue;
		}

		// Mark visited
		pt p = open.front(); open.pop();
		visited[p] = true;

		// Prepare
		int x = p.first;
		int y = p.second;
		Position pos = Position(p.first, p.second).makeValid();

		// Goal test
		if (!isOccupiedPosition(pos) &&
			unit.hasPath(pos))
		{
			return pos;
		}
		// Search
		else
		{
			open.push(pt(x + 1, y));
			open.push(pt(x - 1, y));
			open.push(pt(x, y + 1));
			open.push(pt(x, y - 1));
		}		
	}
	
	// If we couldn't find anything, try again later
	return target;
}
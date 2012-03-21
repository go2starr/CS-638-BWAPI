/* 
 * ActorAgent.cpp
 */
#include "ActorAgent.h"
#include "Agent.h"
#include "Common.h"
#include <BWAPI.h>

using namespace BWAPI;
using std::pair;


ActorAgent::ActorAgent(Unit &u)
	: Agent(u)
{ }

void ActorAgent::update()
{
    switch(state)
    {
    case IdleState: 

		// Invariant to state - If we are moving to an occupied location, adjust target
		if (unit.isMoving())
		{
			Position tp = unit.getTargetPosition();
			if (isOccupiedPosition(tp))
			{
				Broodwar->sendText("Position occupied, adjusting target");
				unit.move(getClosestPosition(tp));
				positionTarget = unit.getTargetPosition();
			}
		}
		break;

    case AttackState: 
	{
		// Attack if we aren't
		if (!unit.isAttacking() && !unit.isMoving())
			unit.attack(positionTarget);
	}

    case DefendState: 
		if (!unit.isAttacking() && !unit.isMoving())
			unit.attack(positionTarget); 
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
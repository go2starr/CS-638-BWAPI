/* 
 * ActorAgent.cpp
 */
#include "ActorAgent.h"
#include "Agent.h"
#include "Common.h"

using namespace BWAPI;


ActorAgent::ActorAgent(Unit &u)
	: Agent(u)
{ }

void ActorAgent::update()
{
    switch(state)
    {
    case IdleState: break;
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
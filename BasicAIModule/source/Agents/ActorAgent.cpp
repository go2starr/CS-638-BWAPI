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
		
		// Draw our attack range
		Position unitPosition = unit.getPosition();
		int x = unitPosition.x();
		int y = unitPosition.y();
		int attackRadius = unit.getType().seekRange();
		Broodwar->drawCircle(CoordinateType::Map, x, y, attackRadius, BWAPI::Colors::White);

		// Draw our current target
		Position targetPosition = unit.getTargetPosition();
		Broodwar->drawLine(CoordinateType::Map, x, y, targetPosition.x(), targetPosition.y(),
			BWAPI::Colors::Red);
		break;
	}

    case DefendState: 
		if (!unit.isAttacking() && !unit.isMoving())
			unit.attack(positionTarget); 
		break;
    }

	Agent::update();
}
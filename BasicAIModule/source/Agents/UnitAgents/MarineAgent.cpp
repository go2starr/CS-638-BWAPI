/*
 * MarineAgent.cpp
 */
#include "MarineAgent.h"
#include "GroundAgent.h"

#include <BWAPI.h>

using BWAPI::Unit;


MarineAgent::MarineAgent(Unit& u)
	: GroundAgent(u)
{
//    state = DefendState;
}

void MarineAgent::update()
{
    switch(state)
    {
    case IdleState: break;
    case AttackState: 
		if (!unit.isAttacking())
			unit.attack(positionTarget); 
		break;
    case DefendState: 
		if (!unit.isAttacking())
			unit.attack(positionTarget); 
		break;
    }

    GroundAgent::update();
}



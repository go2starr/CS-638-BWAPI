/*
 * MarineAgent.cpp
 */
#include "MarineAgent.h"
#include "GroundAgent.h"

#include <BWAPI.h>

using BWAPI::Unit;


MarineAgent::MarineAgent(Unit& u)
	: GroundAgent(u)
{ }

void MarineAgent::update()
{
    switch(state)
    {
    case IdleState: break;
    case AttackState: unit.attack(positionTarget); break;
    case DefendState: unit.patrol(positionTarget); break;
    }

    GroundAgent::update();
}



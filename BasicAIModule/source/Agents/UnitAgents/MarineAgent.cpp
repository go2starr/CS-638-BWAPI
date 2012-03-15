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
    case AttackState:
        unit.attack(positionTarget);
        break;
    }

    GroundAgent::update();
}



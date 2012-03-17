/*
 * MedicAgent.cpp
 */
#include "MedicAgent.h"
#include "GroundAgent.h"

#include <BWAPI.h>

using BWAPI::Unit;


MedicAgent::MedicAgent(Unit& u)
	: GroundAgent(u)
{
    state = DefendState;
}

void MedicAgent::update()
{
    switch(state)
    {
    case IdleState: break;
    case AttackState: unit.attack(positionTarget); break;
    case DefendState: unit.attack(positionTarget); break;
    }
    GroundAgent::update();
}
/*
 * MedicAgent.cpp
 */
#include "UnitAgents/MedicAgent.h"
#include "GroundAgent.h"

#include <BWAPI.h>

using BWAPI::Unit;


MedicAgent::MedicAgent(Unit& u)
	: GroundAgent(u)
{ }

void MedicAgent::update()
{
	if( !unit.isIdle() )
		return;
}

void MedicAgent::setState(ActorState& s)
{
	state = &s;
}


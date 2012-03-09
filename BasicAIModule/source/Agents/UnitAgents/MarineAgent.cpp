/*
 * MarineAgent.cpp
 */
#include "UnitAgents/MarineAgent.h"
#include "GroundAgent.h"

#include <BWAPI.h>

using BWAPI::Unit;


MarineAgent::MarineAgent(Unit& u)
	: GroundAgent(u)
{ }

void MarineAgent::update()
{
	if( !unit.isIdle() )
		return;
}

void MarineAgent::setState(ActorState& s)
{
	state = &s;
}


/*
 * SiegeTankAgent.cpp
 */
#include "UnitAgents/SiegeTankAgent.h"
#include "GroundAgent.h"

#include <BWAPI.h>

using BWAPI::Unit;


SiegeTankAgent::SiegeTankAgent(Unit& u)
	: GroundAgent(u)
{ }

void SiegeTankAgent::update()
{
	if( !unit.isIdle() )
		return;
}

void SiegeTankAgent::setState(ActorState& s)
{
	state = &s;
}


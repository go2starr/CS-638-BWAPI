/*
 * MarineAgent.cpp
 */
#include "MarineAgent.h"
#include "GroundAgent.h"



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
	state = s;
}


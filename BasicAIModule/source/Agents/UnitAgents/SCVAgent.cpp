/*
 * SCVAgent.cpp
 */
#include "Agent.h"
#include "UnitAgents/SCVAgent.h"

SCVAgent::SCVAgent(Unit &u) 
	: GroundAgent(u)
{}

void SCVAgent::setState(ActorState &s) 
{
	this->state = s;
}
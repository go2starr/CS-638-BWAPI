/*
 * GroundAgent.cpp
 */
#include "GroundAgent.h"
#include "ActorAgent.h"

using namespace BWAPI;

GroundAgent::GroundAgent(Unit &u)
	: ActorAgent(u)
{ }

void GroundAgent::update()
{
}

void GroundAgent::setState(ActorState &s)
{
	state = s;
}
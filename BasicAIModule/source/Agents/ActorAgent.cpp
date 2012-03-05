/* 
 * ActorAgent.cpp
 */
#include "ActorAgent.h"
#include "Agent.h"

using namespace BWAPI;

ActorAgent::ActorAgent(Unit &u)
	: Agent(u)
{ }

void ActorAgent::update()
{
}

void ActorAgent::setState(ActorState &s)
{
	// TODO : this split between subclasses of 
	// State might come back to bite us...
	state = s;
}
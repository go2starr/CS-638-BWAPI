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
	Agent::update();
}
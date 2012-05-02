/*
 *  ActorAgent.h
 */
#pragma once
#include "Agent.h"

class ActorAgent : public Agent
{
public:
	virtual void update();
	ActorAgent(BWAPI::Unit &u);
};

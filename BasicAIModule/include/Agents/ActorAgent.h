/*
 *  ActorAgent.h
 */
#pragma once
#include "Agent.h"

class ActorAgent : public Agent
{
public:
	virtual void update();
protected:
	ActorAgent(BWAPI::Unit &u);
};

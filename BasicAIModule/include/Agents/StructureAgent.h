/*
 *  StructureAgent.h
 */
#pragma once
#include "Agent.h"

class StructureAgent: public Agent
{
public:
	virtual void update() { Agent::update(); }
protected:
	StructureAgent(Unit &u) : Agent(u) {}
};

/*
 *  StructureAgent.h
 */
#pragma once
#include "Agent.h"

class StructureAgent: public Agent
{
public:
	virtual void update();
	StructureAgent(BWAPI::Unit &u) : Agent(u) {}
};

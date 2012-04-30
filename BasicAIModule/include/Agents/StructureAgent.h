/*
 *  StructureAgent.h
 */
#pragma once
#include "Agent.h"

// currenty is set up to produce / train agents
// this should be moved to production agent or something
class StructureAgent: public Agent
{
public:
	virtual void update();
	StructureAgent(BWAPI::Unit &u) : Agent(u) {}
};

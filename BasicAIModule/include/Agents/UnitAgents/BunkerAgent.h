/*
 *  BunkerAgent.h
 */
#pragma once
#include "Agent.h"

// currenty is set up to produce / train agents
// this should be moved to production agent or something
class BunkerAgent: public Agent
{
public:
	virtual void update();
	BunkerAgent(BWAPI::Unit &u) : Agent(u) {}
};

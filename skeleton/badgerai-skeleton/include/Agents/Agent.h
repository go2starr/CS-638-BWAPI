#pragma once
#include "AgentState.h"
#include "BWAPIUnit.h"

class Agent
{
protected:
	BWAPIUnit* bwapiUnit;
	AgentState state;

public:
	Agent(void);
	virtual ~Agent(void);
};

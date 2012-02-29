#pragma once
#include "Controller.h"
#include "Agent.h"
#include <vector>

class Manager :	public Controller
{
protected:
	std::vector<Agent*> agents;

public:
	Manager(void);
	~Manager(void);
};

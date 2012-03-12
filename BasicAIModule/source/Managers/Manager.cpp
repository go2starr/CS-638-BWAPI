/*
 *  Manager.cpp - Managers
 */
#include "Manager.h"
#include <Agent.h>
#include <State.h>
#include "Tasks/Task.h"

#include <BWAPI.h>

#include <queue>
#include <set>


using namespace std;
using namespace BWAPI;


Manager::Manager()
	: agents(), tasks(), frames(0)
{ }

void Manager::update()
{ 
	++frames;

	// Tell agents to update
	set<Agent*>::iterator agent;
	for (agent = agents.begin(); agent != agents.end(); agent++)
	{
		(*agent)->update();
	}
}

void Manager::addAgent(Agent &t)
{
	agents.insert(&t);
}

Agent* Manager::removeAgent(BWAPI::UnitType ut)
{
	Agent *agent = NULL;
	set<Agent*>::iterator it;
	for (it = agents.begin(); it != agents.end(); it++)
	{
		if ((*it)->getUnit().getType().getID() == ut.getID())
		{
			agent = *it;
			agents.erase(agents.find(agent));
			break;
		}
	}
	return agent;
}

void Manager::addTask(Task &t)
{
	tasks.push(&t);
}

void Manager::doTask(Task &t)
{

}

int Manager::mwtpNext(UnitType &ut)
{
  return 0;
}

int Manager::mwtpLast(UnitType &t)
{
  return 0;
}

int Manager::estimateCost(Task &t)
{
  return 0;
}

/*
 *  Manager.cpp - Managers
 */
#include "Manager.h"
#include "Agent.h"
#include "States/State.h"
#include "Tasks/Task.h"

#include <BWAPI.h>

#include <queue>
#include <set>


using namespace std;
using namespace BWAPI;


void Manager::update()
{ 
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

void Manager::removeAgent(Agent &t)
{
	agents.erase(agents.find(&t));
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

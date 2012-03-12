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

Agent* Manager::removeAgent(BWAPI::UnitType ut)
{
	Agent *agent = NULL;
	set<Agent*>::iterator it;
	for (it = agents.begin(); it != agents.end(); it++)
	{
		if ((*it)->getUnit().getType().getID() == ut.getID())
		{
			agent = *it;
			agents.erase(agent);
			break;
		}
	}
	return agent;
}

int Manager::numAgents() const { return agents.size(); }

int Manager::numAgents(BWAPI::UnitType ut) const
{
	int count = 0;
	set<Agent*>::const_iterator it  = agents.begin();
	set<Agent*>::const_iterator end = agents.end();
	for(; it != end; ++it)
	{
		if( (*it)->getUnit().getType() == ut )
			++count;
	}
	return count;

void Manager::removeAllAgents()
{
	agents.clear();
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

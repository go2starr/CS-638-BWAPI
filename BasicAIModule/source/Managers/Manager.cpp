/*
 *  Manager.cpp - Managers
 */
#include "Manager.h"
#include "Common.h"
#include "Agent.h"
#include "Tasks/Task.h"

#include <BWAPI.h>

using namespace BWAPI;


/* getAgentsOfType - Gets an AgentSet containing all owned Agents of the specified type, from this Manager's AgentSet */
AgentSet Manager::getAgentsOfType(UnitType type)
{
    return getAgentsOfType(type, agents);
}

/* getAgentsOfType - Gets an AgentSet containing all owned Agents of the specified type from the specified AgentSet */
AgentSet Manager::getAgentsOfType(UnitType type, AgentSet& agentSet)
{
    AgentSet resultSet;
    AgentSetIter it  = agentSet.begin();
    AgentSetIter end = agentSet.end();
    for(; it != end; ++it)
    {
        Agent* agent = *it;
        if( agent->getUnit().getType() ==  type )
            resultSet.insert(agent);
    }
    return resultSet;
}

/* update - Called on each frame */
void Manager::update()
{ 
	// Tell agents to update
	AgentSetIter agent;
	for (agent = agents.begin(); agent != agents.end(); agent++)
	{
		(*agent)->update();
	}
}

/* draw */
void Manager::draw()
{
	for (AgentSetIter agent = agents.begin(); agent != agents.end(); agent++)
		(*agent)->draw();
}

/* addAgent - Add an Agent to the Managers Agent set */
void Manager::addAgent(Agent &t)
{
	agents.insert(&t);
}

/* removeAgent - Remove an Agent of @ut from the Managers Agent set */
Agent* Manager::removeAgent(UnitType ut)
{
	Agent *agent = NULL;
	AgentSetIter it;
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

/* removeAllAgents - Remove all agents 
 * Note: this doesn't free memory allocated for each Agent, 
 * it just dumps the pointers
 */
void Manager::removeAllAgents()
{
	agents.clear();
}

/* addTask  -  Add this task to the task queue */
void Manager::addTask(Task &t)
{
	tasks.push(&t);
}

/*  doTask  -  Do this task without entering the queue */
void Manager::doTask(Task &t)
{

}

/* mwtpNext  -  Value currently placed on accepting one more of this unit type */
int Manager::mwtpNext(UnitType &ut)
{
  return 0;
}

/* mwtpLast  - Value currently placed on the last of this unit type */
int Manager::mwtpLast(UnitType &t)
{
  return 0;
}

/*  estimateCost  -  best estimate at the cost of completing this task */
int Manager::estimateCost(Task &t)
{
  return 0;
}

/* numAgents - Find out how many Agents of any unit type this Manager owns */
int Manager::numAgents() const { return agents.size(); }

/* numAgents - Find out how many Agents of @type this Manager owns */
int Manager::numAgents(UnitType type)
{
    return getAgentsOfType(type).size();
}

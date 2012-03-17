#pragma once
/*
 *  Manager.h  - Managers control Agents.
 */
#include "Common.h"

#include <BWAPI.h>

#include <string>


class Manager
{
protected:
	AgentSet  agents;
	TaskQueue tasks;

    /* getAgentsOfType - Gets an AgentSet containing all owned Agents of the specified type, from this Manager's AgentSet */
    AgentSet getAgentsOfType(BWAPI::UnitType type); 
    /* getAgentsOfType - Gets an AgentSet containing all owned Agents of the specified type, from the specified AgentSet */
    AgentSet getAgentsOfType(BWAPI::UnitType type, AgentSet& agentSet); 

public:    
    /* onMatchStart - Called when a new match begins */
    virtual void onMatchStart() { }

    /* update - Called on each frame */
	virtual void update();

	/* addAgent - Add an Agent to the Managers Agent set */
	virtual void addAgent(Agent &t);
	/* removeAgent - Remove an Agent of @ut from the Managers Agent set */
	virtual Agent* removeAgent(BWAPI::UnitType ut);
	/* removeAllAgents - Remove all agents */
	void removeAllAgents();

	/* addTask  -  Add this task to the task queue */
	virtual void addTask(Task &t);
	/*  doTask  -  Do this task without entering the queue */
	virtual void doTask(Task &t);

	/* mwtpNext  -  Value currently placed on accepting one more of this unit type */
	virtual int mwtpNext(BWAPI::UnitType &ut);
	/* mwtpLast  - Value currently placed on the last of this unit type */
	virtual int mwtpLast(BWAPI::UnitType &ut);
	/*  estimateCost  -  best estimate at the cost of completing this task */
	virtual int estimateCost(Task &t);

    /* numAgents - Find out how many Agents of any unit type this Manager owns */
    int Manager::numAgents() const;
    /* numAgents - Find out how many Agents of @type this Manager owns */
    int numAgents(BWAPI::UnitType type);

    /* getName - Returns an stl string representation of this Manager's name */
    virtual const std::string& getName() const 
    {
        static const std::string name("INVALID");
        return name; 
    }
};




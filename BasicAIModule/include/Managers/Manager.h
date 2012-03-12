/*
 *  Manager.h  - Managers control Agents.
 */
#pragma once
#include <Agent.h>
#include <State.h>
#include "Tasks/Task.h"

#include <BWAPI.h>

#include <queue>
#include <list>
#include <set>

typedef std::set<Agent*> AgentSet;
typedef std::priority_queue<Task*> TaskQueue;

class Manager
{
protected:
	AgentSet  agents;
	TaskQueue tasks;
	unsigned int frames;
  
public:
	/* C'tor */
	Manager();

	/* update - Called on each frame */
	virtual void update();

	/* addAgent - Add an Agent to the Managers Agent set */
	virtual void addAgent(Agent &t);
	/* removeAgent - Remove an Agent of @ut from the Managers Agent set */
	virtual Agent* removeAgent(BWAPI::UnitType ut);

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
};




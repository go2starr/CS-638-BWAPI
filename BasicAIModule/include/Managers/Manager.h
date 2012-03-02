/*
 *  Manager.h  - Managers control Agents.
 */
#pragma once
#include "Agent.h"
#include "State.h"
#include "./Task.h"
#include "queue"

typedef priority_queue<Priority> TaskQueue;

class Manager
{
  std::set<Agent*> agents;
  TaskQueue tasks;
  
public:
  /* update - Called on each frame */
  virtual void update();

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

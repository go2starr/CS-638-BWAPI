/*
 *  Manager.cpp - Managers
 */
#include "Manager.h"
#include "Agent.h"
#include "States/State.h"
#include "Tasks/Task.h"

#include <BWAPI.h>

#include <queue>
#include <list>
#include <set>


using namespace std;
using namespace BWAPI;


void Manager::update()
{ }

void Manager::addAgent(Agent &t)
{
	agents.push_back(t);
}

void Manager::removeAgent(Agent &t)
{
	agents.remove(t);
}

void Manager::addTask(Task &t)
{
	tasks.push(t);
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

/*
 * SCVAgent.cpp
 */
#include "Agent.h"
#include "UnitAgents/SCVAgent.h"

SCVAgent::SCVAgent(Unit &u) 
	: GroundAgent(u)
{}

void SCVAgent::setState(ActorState &s) 
{
	this->state = s;
}

void SCVAgent::update()
{
	// Let them gather resources if not idle
	if (!unit.isIdle())
		return;

	// Just hard-coded to gather closest unoccupied minerals for now..
	// Replace this with a switch on state.getId()
	set<Unit*> minerals = BWAPI::Broodwar->getMinerals();
	int dist = 9999999;
	Unit* closest = 0;
	for (set<Unit*>::iterator i = minerals.begin(); i != minerals.end(); i++)
	{
		Unit *m = *i;
		int d = unit.getDistance(m);
		if (d < dist)
		{
			dist = d;
			closest = m;
		}
	}
	// Gather if we found minerals
	if (closest)
	{
		unit.gather(closest);
	}
}
/*
 *  Strategizer.cpp
 */
#include "Strategizer.h"
#include "UnitAgents/SCVAgent.h"
#include "BWAPI.h"

using namespace BWAPI;
using namespace std;

void Strategizer::update()
{
	// Create agents for newly found, friendly units
	set<Unit*> units = Broodwar->self()->getUnits();
	set<Unit*>::iterator i;
	for (i = units.begin(); i != units.end(); i++)
	{
		Unit *u = *i;
		if (agents.find(u) == agents.end())
		{
			// Insert a new Agent
			if (u->getType().isWorker()) {
				SCVAgent *a = new SCVAgent(*u);
				a->setState(GatherState());
				agents.insert(pair<Unit*, Agent*>(u, a));
			}
		}
	}

	// Tell agents to update
	map<Unit*, Agent*>::iterator j;
	for (j = agents.begin(); j != agents.end(); j++)
	{
		(*j).second->update();
	}
}

void Strategizer::onMatchStart()
{
	Broodwar->sendText("Hello!");
}

void Strategizer::onEvent(JohnConnorBot::Event &e)
{

}



/*
 *  Strategizer.cpp
 */
#include "Strategizer.h"

#include "BuildManager.h"
#include "CombatManager.h"
#include "ConstructionManager.h"
#include "ProductionManager.h"
#include "ResourceManager.h"
#include "ScoutManager.h"
#include "SupplyManager.h"

#include "EventProducer/GameEvent.h"

#include "UnitAgents/SCVAgent.h"

#include "States/GatherState.h"

#include <BWAPI.h>

#include <set>
#include <map>

using namespace BWAPI;
using std::set;
using std::map;


/* 
 * update()
 *
 * Called by the AI module on each frame
 */
void Strategizer::update()
{
	// Create agents for newly found, friendly units
	set<Unit*> units = Broodwar->self()->getUnits();
	set<Unit*>::iterator unit = units.begin();
	set<Unit*>::iterator uend = units.end();
	for (; unit != uend; ++unit)
	{
		Unit *u = *unit;
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
	map<Unit*, Agent*>::iterator it  = agents.begin();
	map<Unit*, Agent*>::iterator end = agents.end();
	for (; it != end; ++it)
		it->second->update();
}

/* 
 * onMatchStart()
 *
 * Called by the AI module when a new match begins
 */
void Strategizer::onMatchStart()
{
	Broodwar->sendText("Hello!");
}

/* 
 * onEvent()
 *
 */
void Strategizer::onEvent(GameEvent &e)
{

}



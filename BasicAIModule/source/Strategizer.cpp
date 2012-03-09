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
	set<Unit*> units = Broodwar->self()->getUnits();
	set<Unit*>::iterator unit;
	map<Unit*, Agent*>::iterator agent;

	// Create agents for newly found, friendly units
	for (unit = units.begin(); unit != units.end(); ++unit)
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

	// Normally, we would shuffle Units around by bid here..
	// Let's just give them to the ResourceManager
	for (agent = agents.begin(); agent != agents.end(); agent++)
	{
		Agent *a = (*agent).second;
		if (a->getUnit().getType().isWorker())
		{
			resourceManager.addAgent(*a);
		}
	}

	// Let Managers update
	buildManager.update();
	combatManager.update();
	constructionManager.update();
	productionManager.update();
	resourceManager.update();
	scoutManager.update();
	supplyManager.update();
}

/* 
 * onMatchStart()
 *
 * Called by the AI module when a new match begins
 */
void Strategizer::onMatchStart()
{
	Broodwar->sendText("Strategizer active.");
}

/* 
 * onEvent()
 *
 */
void Strategizer::onEvent(GameEvent &e)
{
}



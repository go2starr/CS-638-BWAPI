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
#include "UnitAgents/CommandCenterAgent.h"

#include <BWAPI.h>

#include <set>
#include <map>

using namespace BWAPI;
using std::set;
using std::map;
using std::pair;


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
			UnitType ut = u->getType();
			// Insert a new Agent
			if (ut.isWorker()) {
				SCVAgent *a = new SCVAgent(*u);
				agents.insert(pair<Unit*, Agent*>(u, a));
			} else if (ut.isResourceDepot()) {
				CommandCenterAgent *a = new CommandCenterAgent(*u);
				agents.insert(pair<Unit*, Agent*>(u, a));
			}
		}
	}

	// Normally, we would shuffle Units around by bid here..
	for (agent = agents.begin(); agent != agents.end(); agent++)
	{
		Agent *a = (*agent).second;

		// Give SCVs to resource gatherer
		if (a->getUnit().getType().isWorker())
		{
			resourceManager.addAgent(*a);
		}

		// Give command center to production manager
		else if (a->getUnit().getType().isResourceDepot())
		{
			productionManager.addAgent(*a);
		}
	}

	// Let Managers update
	//buildManager.update();
	//combatManager.update();
	//constructionManager.update();
	productionManager.update();
	resourceManager.update();
	//scoutManager.update();
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



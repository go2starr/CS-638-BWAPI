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
#include "UnitAgents/MarineAgent.h"
#include "UnitAgents/CommandCenterAgent.h"
#include "UnitAgents/BarracksAgent.h"

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
	Broodwar->drawTextScreen(300, 0, "\x17 APM=%d", Broodwar->getAPM());

	set<Unit*> units = Broodwar->self()->getUnits();
	set<Unit*>::iterator unit;

	// Create agents for newly found, friendly units
	for (unit = units.begin(); unit != units.end(); ++unit)
	{
		Unit *u = *unit;

		// Only construct active units
		if (!u->isCompleted() ||
			u->getHitPoints() <= 0)
		{
			continue;
		}
		// New agent
		if (unitAgentMap.find(u) == unitAgentMap.end())
		{
			UnitType ut = u->getType();
			Agent *a = NULL;
			if (ut.isWorker()) {
				a = new SCVAgent(*u);
			} else if (ut.isResourceDepot()) {
				a = new CommandCenterAgent(*u);
			} else if (ut == UnitTypes::Terran_Barracks) { 
				a = new BarracksAgent(*u);
			} else if (ut == UnitTypes::Terran_Marine) { 
				a = new MarineAgent(*u);
			}

			if (a != NULL)
				unitAgentMap[u] = a;
		}
	}

	// Normally, we would shuffle Units around by bid here..
	map<Unit*, Agent*>::iterator agent;
	for (agent = unitAgentMap.begin(); agent != unitAgentMap.end(); agent++)
	{
		Agent *a = (*agent).second;

		// Assign SCVs to resource gatherer
		if (a->getUnit().getType().isWorker())
		{
			agentManagerMap[a] = &resourceManager;
		}

		// Give command center to production manager
		else if (a->getUnit().getType().isResourceDepot())
		{
			agentManagerMap[a] = &productionManager;
		}

		// Give Barracks to combat manager
		else if (a->getUnit().getType().getID() == UnitTypes::Terran_Barracks.getID())
		{
			agentManagerMap[a] = &combatManager;
		}

		// Give Marines to combat manager
		else if (a->getUnit().getType().getID() == UnitTypes::Terran_Marine.getID())
		{
			agentManagerMap[a] = &combatManager;
		}
	}

	// If we are running low on supply, give an SCV to the SupplyManager
	if (Broodwar->self()->supplyTotal() - Broodwar->self()->supplyUsed() < 8)
	{
		for (agent = unitAgentMap.begin(); agent != unitAgentMap.end(); agent++)
		{
			Agent *a = (*agent).second;
			if (a->getUnit().getType().isWorker() && agentManagerMap[a] == &resourceManager)
			{
				agentManagerMap[a] = &supplyManager;
				break;
			}
		}
	}

	// If we have enough SCVs, let's try creating a Barracks/Marines
	if (Broodwar->self()->supplyUsed() > 20)
	{
		for (agent = unitAgentMap.begin(); agent != unitAgentMap.end(); agent++)
		{
			Agent *a = (*agent).second;
			if (a->getUnit().getType().isWorker() && agentManagerMap[a] == &resourceManager)
			{
				agentManagerMap[a] = &combatManager;
				break;
			}
		}
	}

	// Revoke all agents from managers
	buildManager.removeAllAgents();
	combatManager.removeAllAgents();
	constructionManager.removeAllAgents();
	productionManager.removeAllAgents();
	resourceManager.removeAllAgents();
	scoutManager.removeAllAgents();
	supplyManager.removeAllAgents();

	// Redistribute agents
	map<Agent*, Manager*>::iterator agentManager;
	for (agentManager = agentManagerMap.begin(); agentManager != agentManagerMap.end(); agentManager++)
	{
		Agent *a = (*agentManager).first;
		Manager *m = (*agentManager).second;
		m->addAgent(*a);
	}	

	// Let Managers update
	//buildManager.update();
	combatManager.update();
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
}

/* 
 * onEvent()
 *
 */
void Strategizer::onEvent(GameEvent &e)
{
}



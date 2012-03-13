/*
 *  Strategizer.cpp
 */
#include "Strategizer.h"
#include "IncludeAllManagers.h"
#include "IncludeAllUnitAgents.h"
#include "GameEvent.h"

#include <BWAPI.h>

#include <set>
#include <map>

using namespace BWAPI;
using std::set;
using std::map;
using std::pair;


/* 
 *  update()
 *
 *  Called by the AI module on each frame
 */
void Strategizer::update()
{
	Broodwar->drawTextScreen(300, 0, "\x17 APM=%d", Broodwar->getAPM());

	// Find new units, remove inactive ones
	updateUnitAgentMap();

	// Remap Agents to Managers (bid war)
	updateAgentManagerMap();	

	// Give Agents to updated Managers
	redistributeAgents();

	// Let Managers manager
	updateManagers();
}

/* 
 *  onMatchStart()
 *
 *  Called by the AI module when a new match begins
 */
void Strategizer::onMatchStart()
{
}

/* 
 *  onEvent()
 *
 */
void Strategizer::onEvent(GameEvent &e)
{
}

/*
 * onMatchEnd()
 *
 * Called by the AI module when a match ends
 */
void Strategizer::onMatchEnd(bool isWinner)
{
    // Cleanup agents
    // Note: this is the safe way to do this
    // erasing by iterator invalidates the iterator, 
    // so if we just use map.erase(it), the loop gets screwed up, 
    // postfix increment inside the loop keeps the 
    // iterator valid as it traverses the map
    // Source: Effective STL (Scott Meyers - 2001)
	map<Agent*, Manager*>::iterator it  = agentManagerMap.begin();
    map<Agent*, Manager*>::iterator end = agentManagerMap.end();
    for(; it != end;)
    {
        Agent* agent = it->first;
        agentManagerMap.erase(it++);
        delete agent;
    }
}


/*
 *  updateUnitAgentMap()
 */
void Strategizer::updateUnitAgentMap()
{
	set<Unit*> units = Broodwar->self()->getUnits();
	set<Unit*>::iterator unit;

	// Create agents for newly found, friendly units
	for (unit = units.begin(); unit != units.end(); ++unit)
	{
		Unit *u = *unit;

		// Only construct active units
		if (!u->isCompleted())  // TODO: Determine a more robust conditional
		{
			continue;
		}
		// New agent
		if (unitAgentMap.find(u) == unitAgentMap.end())
		{
			UnitType ut = u->getType();
			Agent *a = NULL;

			if (ut.isWorker()) 				a = new SCVAgent(*u);
			else if (ut.isResourceDepot())	a = new CommandCenterAgent(*u);
            else if (ut == UnitTypes::Terran_Refinery)
                a = new RefineryAgent(*u);
			else if (ut == UnitTypes::Terran_Barracks) 
				a = new BarracksAgent(*u);
			else if (ut == UnitTypes::Terran_Marine) 
				a = new MarineAgent(*u);
            else if (ut == UnitTypes::Terran_Firebat) 
                a = new FirebatAgent(*u);
            else if (ut == UnitTypes::Terran_Medic)
                a = new MedicAgent(*u);

			if (a != NULL)
				unitAgentMap[u] = a;
		}
	}

	// TODO: Cleanup inactive agents
}


/*
 *  updateAgentManagerMap()
 */
void Strategizer::updateAgentManagerMap()
{
	// Normally, we would shuffle Units around by bid here..
	map<Unit*, Agent*>::iterator agent;
	for (agent = unitAgentMap.begin(); agent != unitAgentMap.end(); agent++)
	{
		Agent   *a  = (*agent).second;
        UnitType ut = a->getUnit().getType();

		// Assign SCVs to resource gatherer
		if (a->getUnit().getType().isWorker())
			agentManagerMap[a] = &resourceManager;
        // Give refinery to resource manager
        else if (ut.isRefinery())
            agentManagerMap[a] = &resourceManager;
		// Give command center to production manager
		else if (ut.isResourceDepot())
			agentManagerMap[a] = &productionManager;
		// Give Barracks to combat manager
		else if (ut == UnitTypes::Terran_Barracks)
			agentManagerMap[a] = &combatManager;
		// Give Marines to combat manager
		else if (ut == UnitTypes::Terran_Marine)
			agentManagerMap[a] = &combatManager;
        // Give Firebats to combat manager
        else if (ut == UnitTypes::Terran_Firebat)
			agentManagerMap[a] = &combatManager;
	}

	// If we are running low on supply, give an SCV to the SupplyManager
    const int remainingSupply = Broodwar->self()->supplyTotal() 
                              - Broodwar->self()->supplyUsed();
	if (remainingSupply < 8)
	{
		for (agent = unitAgentMap.begin(); agent != unitAgentMap.end(); agent++)
		{
			Agent   *a  = (*agent).second;
            UnitType ut = a->getUnit().getType();

			if (ut.isWorker() && agentManagerMap[a] == &resourceManager)
			{
				agentManagerMap[a] = &supplyManager;
				break;
			}
		}
	}

	// If we have enough SCVs, let's try creating a Barracks/Army
	if (Broodwar->self()->supplyUsed() > 20 &&
		combatManager.numAgents(BWAPI::UnitTypes::Terran_SCV) <= 1)
	{
		for (agent = unitAgentMap.begin(); agent != unitAgentMap.end(); agent++)
		{
			Agent   *a  = (*agent).second;
            UnitType ut = a->getUnit().getType();
			if (ut.isWorker() && agentManagerMap[a] == &resourceManager)
			{
				agentManagerMap[a] = &combatManager;
				break;
			}
		}
	}
}

/*
 *  redistributeAgents()
 */
void Strategizer::redistributeAgents()
{
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
		Agent   *a = (*agentManager).first;
		Manager *m = (*agentManager).second;
		m->addAgent(*a);
	}
}

void Strategizer::updateManagers()
{
	//buildManager.update();
	combatManager.update();
	//constructionManager.update();
	productionManager.update();
	resourceManager.update();
	//scoutManager.update();
	supplyManager.update();
}


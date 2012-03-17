/*
*  Strategizer.cpp
*/
#include "Strategizer.h"
#include "IncludeAllManagers.h"
#include "IncludeAllUnitAgents.h"
#include "TacticalBuildingPlacer.h"
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
	// Draw "GUI"
	Broodwar->drawTextScreen(300, 0, "\x17 APM=%d", Broodwar->getAPM());
	TacticalBuildingPlacer::instance().update(); // draw reserved map

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
    buildManager.onMatchStart();
    combatManager.onMatchStart();
    gasManager.onMatchStart();
    productionManager.onMatchStart();
    resourceManager.onMatchStart();
    scoutManager.onMatchStart();
    supplyManager.onMatchStart();

	// Barracks do not ever leave idle state (for now), so 1 per unit type

	// Marines
	buildManager.build(UnitTypes::Terran_Barracks);
	buildManager.build(UnitTypes::Terran_Marine);

	// Upgrade
	buildManager.build(UnitTypes::Terran_Academy);

	// Firebat
	buildManager.build(UnitTypes::Terran_Barracks);
	buildManager.build(UnitTypes::Terran_Firebat);

	// Medic
	buildManager.build(UnitTypes::Terran_Barracks);
	buildManager.build(UnitTypes::Terran_Medic);


	/******** FOR GOOD MEASURE :-) ***********************/
	// Marines
	buildManager.build(UnitTypes::Terran_Barracks);
	buildManager.build(UnitTypes::Terran_Marine);

	// Upgrade
	buildManager.build(UnitTypes::Terran_Academy);

	// Firebat
	buildManager.build(UnitTypes::Terran_Barracks);
	buildManager.build(UnitTypes::Terran_Firebat);

	// Marines
	buildManager.build(UnitTypes::Terran_Barracks);
	buildManager.build(UnitTypes::Terran_Marine);

	// Upgrade
	buildManager.build(UnitTypes::Terran_Academy);

	// Firebat
	buildManager.build(UnitTypes::Terran_Barracks);
	buildManager.build(UnitTypes::Terran_Firebat);


	/****************************************************
	// Upgrade - Factory
	buildManager.build(UnitTypes::Terran_Factory);

	// Vulture
	buildManager.build(UnitTypes::Terran_Barracks);
	buildManager.build(UnitTypes::Terran_Vulture);

	// Upgrade
	buildManager.build(UnitTypes::Terran_Armory);

	// Goliaths
	buildManager.build(UnitTypes::Terran_Barracks);
	buildManager.build(UnitTypes::Terran_Goliath);
	****************************************************/
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
*
*  Maps new friendly units to agents
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

			if (ut.isWorker()) 				
				a = new SCVAgent(*u);
			else if (ut.isResourceDepot())	
				a = new CommandCenterAgent(*u);
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
*
*  Maps agents to managers based upon which managers
*  need the agent
*/
void Strategizer::updateAgentManagerMap()
{
	// Normally, we would shuffle Units around by bid here..
	map<Unit*, Agent*>::iterator agent;
	for (agent = unitAgentMap.begin(); agent != unitAgentMap.end(); agent++)
	{
		Agent   *a  = (*agent).second;
		UnitType ut = a->getUnit().getType();

		// if Agent hasn't been assigned a manager
		if (agentManagerMap[a] == NULL) {

			// Resources:
			// SCV -> ResourceManager
			if (a->getUnit().getType().isWorker())
				agentManagerMap[a] = &resourceManager;
			// Refinery -> Gas Manager
			else if (ut.isRefinery())
				agentManagerMap[a] = &gasManager;
			// Command Center -> Production Manager
            // TODO - this is the wrong ProductionManager
            // we want to assign it to the one in BuildManager
			else if (ut.isResourceDepot())
				agentManagerMap[a] = &productionManager;

			// Army:
			// Barracks -> BuildManager
			else if (ut == UnitTypes::Terran_Barracks)
				agentManagerMap[a] = &buildManager;

			// Combat:
			// Marines -> CombatManager
			else if (ut == UnitTypes::Terran_Marine)
				agentManagerMap[a] = &combatManager;
			// Firebat -> CombatManager
			else if (ut == UnitTypes::Terran_Firebat)
				agentManagerMap[a] = &combatManager;
            // Medic -> CombatManager
            else if (ut == UnitTypes::Terran_Medic)
                agentManagerMap[a] = &combatManager;
		}
	}

	// If we are running low on supply, give an SCV to the SupplyManager
	/*
	const int remainingSupply = Broodwar->self()->supplyTotal() - Broodwar->self()->supplyUsed();
	if (remainingSupply < 6 && supplyManager.numAgents(UnitTypes::Terran_SCV) < 1)
	{
		remap(UnitTypes::Terran_SCV, resourceManager, supplyManager);
	}
	*/

	// If we have enough SCVs, let's try creating a Barracks/Army
	if (Broodwar->self()->supplyUsed() >= 20 &&
		combatManager.numAgents(BWAPI::UnitTypes::Terran_SCV) < 1 )
	{
		remap(UnitTypes::Terran_SCV, resourceManager, combatManager);
	}

	// take one of the resourceManager SCV's and give it to the gas manager
	if (Broodwar->self()->supplyUsed() >= 30 &&
		gasManager.numAgents(BWAPI::UnitTypes::Terran_SCV) < 1)
	{
		remap(UnitTypes::Terran_SCV, resourceManager, gasManager);
	}

	// Give an SCV to the BuildManager
	if (buildManager.numAgents(BWAPI::UnitTypes::Terran_SCV) < 1)
	{
		remap(UnitTypes::Terran_SCV, resourceManager, buildManager);
	}
}

/*
*  redistributeAgents()
*
*  This is what actually gives the managers their agents
*  and takes agents away from other managers that have
*  been re-assigned based upon the agentManagerMap
*/
void Strategizer::redistributeAgents()
{
	// Revoke all agents from managers
	buildManager.removeAllAgents();
	combatManager.removeAllAgents();
	gasManager.removeAllAgents();
	productionManager.removeAllAgents(); // remove once build mgr is more complete
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
        a->setParentManager(m);
	}
}

void Strategizer::updateManagers()
{
	buildManager.update();
	combatManager.update();
	gasManager.update();
	productionManager.update(); // remove once build mgr is more complete
	resourceManager.update();
	//scoutManager.update();
	supplyManager.update();
}

bool Strategizer::remap(BWAPI::UnitType type, Manager &src, Manager &dst)
{
	map<Unit*, Agent*>::iterator i;
	for (i = unitAgentMap.begin(); i != unitAgentMap.end(); i++)
		{
			Agent   *a  = (*i).second;
			UnitType ut = a->getUnit().getType();
			if (ut.getID() == type.getID() && agentManagerMap[a] == &src)
			{
				agentManagerMap[a] = &dst;
				return false;
			}
		}
	return true;
}


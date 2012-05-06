/*
*  Strategizer.cpp
*/
#include "Strategizer.h"
#include "IncludeAllManagers.h"
#include "IncludeAllAdvisors.h"
#include "IncludeAllUnitAgents.h"
#include "TacticalBuildingPlacer.h"
#include "DebugFlags.h"
#include "GameEvent.h"
#include "Common.h"

#include <BWAPI.h>

using namespace BWAPI;

using std::string;
using std::pair;


/* 
*  update()
*
*  Called by the AI module on each frame
*/
void Strategizer::update()
{
	TacticalBuildingPlacer::instance().update();

	// Draw managers
	draw();

	if (Broodwar->getFrameCount() % 10 == 0)
	{
		// Find new units, remove inactive ones
		updateUnitAgentMap();
	
		// Remap Agents to Managers (bid war)
		updateAgentManagerMap();	

		// Give Agents to updated Managers
		redistributeAgents();
	
		// Let Managers manager
		updateManagers();

		// Update any advisors that might need updating
		SupplyAdvisor::update();
		MapAdvisor::update();

		// Forfeit the match if conditions warrant
        if( checkForfeit() )
        {
            Broodwar->sendText("Surrendering match...");
            Broodwar->leaveGame();
        }
	}
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
    scoutManager.onMatchStart();

	MapAdvisor::init(Broodwar->mapWidth(), Broodwar->mapHeight());

	// Start off building SCVs
	buildManager.build(UnitTypes::Terran_SCV, false, 20);

	// Barracks do not ever leave idle state (for now), so 1 per unit type
	// Initial troops
	buildManager.build(UnitTypes::Terran_Barracks);
	buildManager.build(UnitTypes::Terran_Marine, false, 40);
	buildManager.build(UnitTypes::Terran_Bunker);
	buildManager.build(UnitTypes::Terran_Barracks);
	buildManager.build(UnitTypes::Terran_Marine, false, 30);
	buildManager.build(UnitTypes::Terran_Bunker);
	buildManager.build(UnitTypes::Terran_Bunker);

	// Expand
	buildManager.build(UnitTypes::Terran_Command_Center);

	// Secondary troops
	buildManager.build(UnitTypes::Terran_Barracks);
	buildManager.build(UnitTypes::Terran_Firebat, false, 30);
	buildManager.build(UnitTypes::Terran_Factory);
	buildManager.build(UnitTypes::Terran_Vulture, false, 30);

	buildManager.build(UnitTypes::Terran_Factory);
	buildManager.build(UnitTypes::Terran_Siege_Tank_Tank_Mode, false, 30);

	// Expand
	buildManager.build(UnitTypes::Terran_Command_Center);

	buildManager.build(UnitTypes::Terran_Goliath.whatBuilds().first);	
	//buildManager.build(UnitTypes::Terran_Goliath.whatBuilds().first);
	buildManager.build(UnitTypes::Terran_Goliath, false, 30);
	//buildManager.build(UnitTypes::Terran_Goliath);

	buildManager.build(UnitTypes::Terran_Starport);
	buildManager.build(UnitTypes::Terran_Battlecruiser, false, 30);
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
    combatManager.onMatchEnd(isWinner);

	// Cleanup agents
	// Note: this is the safe way to do this
	// erasing by iterator invalidates the iterator, 
	// so if we just use map.erase(it), the loop gets screwed up, 
	// postfix increment inside the loop keeps the 
	// iterator valid as it traverses the map
	// Source: Effective STL (Scott Meyers - 2001)
	AgentManagerMapIter it  = agentManagerMap.begin();
	AgentManagerMapIter end = agentManagerMap.end();
	for(; it != end;)
	{
		Agent* agent = it->first;
		agentManagerMap.erase(it++);
		delete agent;
	}

	scoutManager.onMatchEnd();
	MapAdvisor::onMatchEnd();
}

/*
 * onUnitDiscovered()
 * 
 * Called by the AI module when a new unit is first discovered
 */
void Strategizer::onUnitDiscovered( Unit* unit )
{
    const string& name   = unit->getType().getName();
    const int     typeID = unit->getType().getID();
    const bool    isSelf = unit->getPlayer() == Broodwar->self();

    if( typeID == UnitTypes::Resource_Mineral_Field 
     || typeID == UnitTypes::Resource_Mineral_Field_Type_2
     || typeID == UnitTypes::Resource_Mineral_Field_Type_3 )
    {
		ResourceAdvisor::discoverMineralPatch(*unit);
        return;
    }

    if( typeID == UnitTypes::Resource_Vespene_Geyser )
    {
		ResourceAdvisor::discoverVespeneGeyser(*unit);
        return;
    }

    if( isSelf ) 
    {
        // TODO: pass along any of our units on to whoever might need them
    }
    else
    {
        combatManager.discoverEnemyUnit(unit);
    }
}

/*
*  updateUnitAgentMap()
*
*  Maps new friendly units to agents
*/
void Strategizer::updateUnitAgentMap()
{
	// Create agents for newly found, friendly units.  Delete dead ones
    UnitSet units = Broodwar->self()->getUnits();
	for (UnitSetIter unit = units.begin(); unit != units.end(); ++unit)
	{
		Unit *u = *unit;

		// Only construct active units
		if (!u->isCompleted())  // TODO: Determine a more robust conditional
		{
			continue;
		}
		// New agent
		if (unitAgentMap[u] == NULL)
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
			// bunker has it's own agent
			else if (ut.isBuilding() && ut != UnitTypes::Terran_Bunker)
				a = new StructureAgent(*u);
			else if (ut.isAddon())
				a = new StructureAgent(*u);
			else if (ut == UnitTypes::Terran_Bunker)
				a = new BunkerAgent(*u);
			else
				a = new ActorAgent(*u);

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
	for (UnitAgentMapIter agent = unitAgentMap.begin(); agent != unitAgentMap.end(); agent++)
	{
		Agent   *a  = (*agent).second;
		UnitType ut = a->getUnit().getType();

		// if Agent hasn't been assigned a manager
		if (agentManagerMap[a] == NULL) {

			// Note: apparently we're only really going to be 
			// making use of the Build/Combat managers...

			// Resources:
			// SCV -> Build Manager 
			if (a->getUnit().getType().isWorker())
				agentManagerMap[a] = &buildManager;
			// Refinery -> Gas Manager
			else if (ut.isRefinery())
				agentManagerMap[a] = &gasManager;

			// Command Center -> Build Manager
			else if (ut.isResourceDepot())
				agentManagerMap[a] = &buildManager;

			// Army:
			// Barracks -> BuildManager
			else if (ut == UnitTypes::Terran_Barracks)
				agentManagerMap[a] = &buildManager;
			// bunker agent goes to the combat manager
			else if (ut.isBuilding() && ut != UnitTypes::Terran_Bunker)
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
			// Bunker -> CombatManager
			else if (ut == UnitTypes::Terran_Bunker)
				agentManagerMap[a] = &combatManager;
			else
				agentManagerMap[a] = &combatManager;
		}
	}

	// take three of the resourceManager SCV's and give it to the gas manager
	if (Broodwar->self()->supplyUsed() >= 30 &&
		gasManager.numAgents(UnitTypes::Terran_SCV) < 3)
	{
		remap(UnitTypes::Terran_SCV, buildManager, gasManager);
	}

	// take one of the resourceManger's SCV's and give to the scout manager
	if (Broodwar->self()->supplyUsed() >= 20 && 
		scoutManager.numAgents(UnitTypes::Terran_SCV) < 1)
	{
		remap(UnitTypes::Terran_SCV, buildManager, scoutManager);
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
	scoutManager.removeAllAgents();

	// Redistribute agents
    AgentManagerMapIter it  = agentManagerMap.begin();
    AgentManagerMapIter end = agentManagerMap.end();
	for(; it != end; ++it)
	{
        pair<Agent*, Manager*> agentManager = *it;
		Agent   *a = agentManager.first;
		Manager *m = agentManager.second;
		// TODO : Change this to assert
		if (a != NULL)
		{
			m->addAgent(*a);
			a->setParentManager(m);
		}
	}
}

void Strategizer::updateManagers()
{
	buildManager.update();
	combatManager.update();
	gasManager.update();
	scoutManager.update();
}


void Strategizer::draw()
{
	buildManager.draw();
	combatManager.draw();
	gasManager.draw();
	scoutManager.draw();

	DebugFlags::instance().draw();
}

bool Strategizer::remap(BWAPI::UnitType type, Manager &src, Manager &dst)
{
    UnitAgentMapIter it  = unitAgentMap.begin();
    UnitAgentMapIter end = unitAgentMap.end();
	for(; it != end; ++it)
	{
        pair<Unit*, Agent*> unitAgent = *it;
		Agent   *a  = unitAgent.second;
		UnitType ut = a->getUnit().getType();
		if (ut == type && agentManagerMap[a] == &src)
		{
			agentManagerMap[a] = &dst;
			return false;
		}
	}
	return true;
}

bool Strategizer::checkForfeit()
{
    AgentSet pmCC(buildManager.getAgentsOfType(UnitTypes::Terran_Command_Center));
    return   pmCC.empty();
}

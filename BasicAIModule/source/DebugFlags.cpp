/*
 * DebugFlags.cpp
 */
#include "DebugFlags.h"
#include "Strategizer.h"
#include "IncludeAllAdvisors.h"
#include "IncludeAllManagers.h"

#include <BWAPI.h>

#include <string>

using namespace BWAPI;

const std::string DebugFlagStrings[] = 
{
	"drawNone", 
	"drawSystemDetails",
	"drawSquadDetails",
	"drawAgentDetails", 
	"drawAgentTargets", 
	"drawBuildQueue", 
	"drawBuildStack",
	"drawReserveMap",
	"drawMapBounds"
};


DebugFlags::DebugFlags()
: drawNone          (false)
, drawSystemDetails (true)
, drawSquadDetails  (true)
, drawAgentDetails  (true)
, drawAgentTargets  (false)
, drawBuildQueue    (false)
, drawBuildStack    (false)
, drawReserveMap    (false)
, drawMapBounds     (false)
{ }

void DebugFlags::draw()
{
	if( drawNone )
		return;

	Strategizer& strategizer = Strategizer::instance();

	if( drawSystemDetails )
	{
		// Supply Advisor
		Broodwar->drawTextScreen( 2, 0, 
			"\x1E SM : %d planned"
			, SupplyAdvisor::plannedSupply() / 2 );

		// Build Manager
		BuildManager& bm = strategizer.buildManager;
		Broodwar->drawTextScreen( 2, 40, 
			"\x07 BM : (SCV=%d) (CC=%d) (MS=%d)"
			, bm.numAgents(UnitTypes::Terran_SCV)
			, bm.numAgents(UnitTypes::Terran_Command_Center)
			, bm.numAgents(UnitTypes::Terran_Machine_Shop) );

		// Combat Manager
		CombatManager& cm = strategizer.combatManager;
		Broodwar->drawTextScreen( 2, 20, 
			"\x11 CM : (Enemies=%d) (Squads=%d) : (Mar=%d) (Fire=%d) (Med=%d)"
			, cm.numEnemyUnits()
			, cm.numSquads()
			, cm.numAgents(UnitTypes::Terran_Marine) 
			, cm.numAgents(UnitTypes::Terran_Firebat) 
			, cm.numAgents(UnitTypes::Terran_Medic) );

		// Scout Manager
		ScoutManager& sm = strategizer.scoutManager;
		Broodwar->drawTextScreen( 2, 50, 
			"\x10 SM : (SCV=%d)"
			, sm.numAgents(UnitTypes::Terran_SCV) );

		// Gas Manager
		GasManager& gm = strategizer.gasManager;
		Broodwar->drawTextScreen( 2, 30, 
			"\x10 GM : (SCV=%d)"
			, gm.numAgents(UnitTypes::Terran_SCV) );

		// Draw "GUI"
		Broodwar->drawTextScreen(300, 0, "\x17 APM=%d", Broodwar->getAPM());
		Broodwar->drawTextScreen(300,10, "\x17 FPS=%d", Broodwar->getFPS());
	}

// 	if( drawSquadDetails )
// 	{
// 		// Handled in Squad::draw()
// 	}

// 	if( drawAgentDetails )
// 	{
// 		// Handled in Agent::draw()
// 	}

// 	if( drawAgentTargets )
// 	{
// 		// Handled in Agent::draw()
// 	}

	// TODO: split this, drawing them individually
	if( drawBuildQueue || drawBuildStack )
	{
		strategizer.buildManager.drawDebugText();
	}

// 	if( drawReserveMap )
// 	{
// 		// Handled in TacticalBuildingPlacer::update()/draw()
// 	}

// 	if( drawMapBounds )
// 	{
// 		// Handled in EnhancedUI::update()
// 	}
}

bool DebugFlags::getFlag( const DebugFlag& flag ) const
{
	bool state = false;
	switch(flag)
	{
	case none:           state = drawNone;          break;
	case system_details: state = drawSystemDetails; break;
	case squad_details:  state = drawSquadDetails;  break;
	case agent_details:  state = drawAgentDetails;  break;
	case agent_targets:  state = drawAgentTargets;  break;
	case build_queue:    state = drawBuildQueue;    break;
	case build_stack:    state = drawBuildStack;    break;
	case reserve_map:    state = drawReserveMap;    break;
	case map_bounds:     state = drawMapBounds;     break;
	}
	return state;
}

void DebugFlags::setFlag( const DebugFlag& flag, const bool state )
{
	switch(flag)
	{
	case none:           drawNone          = state; break;
	case system_details: drawSystemDetails = state; break;
	case squad_details:  drawSquadDetails  = state; break;
	case agent_details:  drawAgentDetails  = state; break;
	case agent_targets:  drawAgentTargets  = state; break;
	case build_queue:    drawBuildQueue    = state; break;
	case build_stack:    drawBuildStack    = state; break;
	case reserve_map:    drawReserveMap    = state; break;
	case map_bounds:     drawMapBounds     = state; break;
	}
}

void DebugFlags::toggleFlag( const DebugFlag& flag )
{
	switch(flag)
	{
	case none:           drawNone          = !drawNone;          break;
	case system_details: drawSystemDetails = !drawSystemDetails; break;
	case squad_details:  drawSquadDetails  = !drawSquadDetails;  break;
	case agent_details:  drawAgentDetails  = !drawAgentDetails;  break;
	case agent_targets:  drawAgentTargets  = !drawAgentTargets;  break;
	case build_queue:    drawBuildQueue    = !drawBuildQueue;    break;
	case build_stack:    drawBuildStack    = !drawBuildStack;    break;
	case reserve_map:    drawReserveMap    = !drawReserveMap;    break;
	case map_bounds:     drawMapBounds     = !drawMapBounds;     break;
	}
}

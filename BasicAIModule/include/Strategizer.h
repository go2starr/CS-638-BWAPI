/* 
 * Strategizer.h
 */
#pragma once
#include "Managers/Manager.h"
#include "GameEvent.h"
// #include "GameStateDB.h"

#include "BuildManager.h"
	#include "CombatManager.h"
#include "ConstructionManager.h"
#include "ProductionManager.h"
#include "ResourceManager.h"
#include "ScoutManager.h"
#include "SupplyManager.h"

#include <BWAPI.h>

#include <map>


class Strategizer
{
	// Managers
	BuildManager         buildManager;
	CombatManager        combatManager;
	ConstructionManager  constructionManager;
	ProductionManager    productionManager;
	ResourceManager      resourceManager;
	ScoutManager         scoutManager;
	SupplyManager        supplyManager;

	// Agents
	std::map<BWAPI::Unit*, Agent*> agents;

	// Game State analysis
	// GameStateDB gsdb;

public:
	
	/* Strategizer is a Singleton class, access is through it's instance method */
	static Strategizer& instance() { static Strategizer s; return s; }

	/* update - Called by AI Module for each frame */
	void update();

	/* onMatchStart - Called by AI Module when a new match begins */
	void onMatchStart();

	/* onEvent - Called by EventProducer when a new event is detected */
	void onEvent(GameEvent& e);
  
private:
  
	// Strategizer is singleton, hence private ctors/assignment
	Strategizer() { }
	Strategizer(const Strategizer& other);
	void operator=(const Strategizer& other);
};



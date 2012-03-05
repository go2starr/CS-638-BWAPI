/* 
 * Strategizer.h
 */
#pragma once
#include <BWAPI.h>
#include "Managers/Manager.h"
#include "Event.h"
// #include "GameStateDB.h"

// Managers
#include "BuildManager.h"
#include "ResourceManager.h"
#include "CombatManager.h"
#include "ScoutManager.h"
#include "ConstructionManager.h"
#include "SupplyManager.h"
#include "ProductionManager.h"

// States
#include "States/GatherState.h"

using namespace BWAPI;
using namespace std;

class Strategizer
{
  // Managers
  BuildManager buildManager;
  CombatManager combatManager;
  ConstructionManager constructionManager;
  ProductionManager productionManager;
  ResourceManager resourceManager;
  ScoutManager scoutManager;
  SupplyManager supplyManager;

  // Agents
  map<Unit*, Agent> agents;

  // Game State analysis
  // GameStateDB gsdb;

public:
  
  void update();
  static Strategizer& instance() { static Strategizer s; return s; }

	/* onMatchStart - Called by AI Module when a new match begins */
	void onMatchStart();

  /* onEvent - Called by EventProducer when a new event is detected */
  void onEvent(JohnConnorBot::Event &e);
  
private:
  
  // Strategizer is singleton, hence private ctors/assignment
  Strategizer() {}
  Strategizer(const Strategizer& other);
  void operator=(const Strategizer& other);
};



/* 
 * Strategizer.h
 */
#pragma once
#include "Managers/Manager.h"
#include "GameStateDB.h"
#include "EventProducer/Event.h"

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

  // Game State analysis
  GameStateDB gsdb;

public:
  
  void update();
  static Strategizer& instance() { static Strategizer s; return s; }

  /* onEvent - Called by EventProducer when a new event is detected */
  void onEvent(Event e);
  
private:
  
  // Strategizer is singleton, hence private ctors/assignment
  Strategizer();
  Strategizer(const Strategizer& other);
  void operator=(const Strategizer& other);

};

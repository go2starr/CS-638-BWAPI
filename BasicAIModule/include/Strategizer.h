/* 
 * Strategizer.h
 */
#pragma once


class Strategizer
{
  BuildManager buildManager;
  CombatManager combatManager;
  ConstructionManager constructionManager;
  ProductionManager productionManager;
  ResourceManager resourceManager;
  ScoutManager scoutManager;
  SupplyManager supplyManager;

public:

  void update();
  static Strategizer& instance() { static Strategizer s; return s; }

private:

	// Strategizer is singleton, hence private ctors/assignment
	Strategizer();
	Strategizer(const Strategizer& other);
	void operator=(const Strategizer& other);

};

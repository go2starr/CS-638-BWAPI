/*
 *  Arbitrator.h - Used to redistribute Units and allocate newly created Units
 */
#pragma once

#include "Managers/Manager.h"

class Arbitrator
{
  std::set<Manager> managers;   // Bidding Managers
  std::set<Agent> agents;       // Agents to provide

public:
  /* 
   * update - Update calls resolve() until newly created
   * Units are assigned, and MWTP from Managers is at equillibrium
   */
  void update();

private:
  /*
   *  resolve - Transfers available Agents to Managers with highest
   *  willingness to pay.
   */
  bool resolve();

  /*
   *  findNewUnits() - Creates Agents for each newly produced Unit,
   *  and adds them to the set of managed Agents.
   */
  void findNewUnits();

};



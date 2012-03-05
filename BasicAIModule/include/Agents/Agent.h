/*
 *  Agent.h 
 */
#pragma once
#include <BWAPI.h>
#include "States/State.h"

using namespace BWAPI;

class Agent 
{
protected:
  Unit &unit;					// The Unit this Agent controls
  State state;					// The State this Agent is in

  // D'tor
  ~Agent();
  
public:
  /*
   * update - Called on each frame to update this agent's state and unit.
   */
  virtual void update();

  // attr_accessors
  State& getState();
};

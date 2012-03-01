/*
 *  Agent.h 
 */
#pragma once
#include "../BasicAIModule.h"
#include "../States/State.h"

class Agent 
{
protected:
  BWAPI::Unit &unit;

  // C'tor
  Agent(BWAPI::Unit);

  // D'tor
  ~Agent();

public:
  /*
   * update - Called on each frame to update this agent's state and unit.
   */
  void update();

  // attr_accessors
  void setState(const State &state);
  void getState(State &state);
};

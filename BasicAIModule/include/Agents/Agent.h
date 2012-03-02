/*
 *  Agent.h 
 */
#pragma once
#include "../BasicAIModule.h"
#include "../States/State.h"

class Agent 
{
protected:
  BWAPI::Unit &unit;            // The Unit this Agent controls
  State state;                  // The State this Agent is in

  // C'tor
  Agent(BWAPI::Unit &unit);
  
  // D'tor
  ~Agent();
  
public:
  /*
   * update - Called on each frame to update this agent's state and unit.
   */
  virtual void update();

  // attr_accessors
  void setState(State &s);
  State& getState();
};

/*
 *  ScoutState.h
 */
#pragma once
#include "State.h"

class ScoutState: public ActorState
{
public:
  ScoutState(PositionTarget &t);
  virtual void setTarget(PositionTarget &t);
  virtual PositionTarget& getTarget(); 
};

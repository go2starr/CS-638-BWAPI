/*
 *  EvadeState.h
 */
#pragma once
#include "State.h"

class EvadeState: public ActorState
{
public:
  EvadeState();
  virtual PositionTarget& getTarget(); // Evading to
};

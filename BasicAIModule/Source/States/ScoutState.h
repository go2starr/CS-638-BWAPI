/*
 *  ScoutState.h
 */
#pragma once
#include "State.h"

class ScoutState: public State
{
public:
  ScoutState();
  ScoutState(PositionTarget &t);
  virtual void onEnter();
  virtual void onUpdate();
  virtual void onExit();
};

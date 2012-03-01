/*
 *  DefendState.h
 */
#pragma once
#include "State.h"

class DefendState: public State
{
public:
  DefendState(UnitTarget &t);
  virtual void onEnter();
  virtual void onUpdate();
  virtual void onExit();
};

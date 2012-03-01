/*
 *  EvadeState.h
 */
#pragma once
#include "State.h"

class EvadeState: public State
{
public:
  EvadeState();
  virtual void onEnter();
  virtual void onUpdate();
  virtual void onExit();
};

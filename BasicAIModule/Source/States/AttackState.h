/*
 *  AttackState.h
 */
#pragma once
#include "State.h"

class AttackState: public State
{
public:
  AttackState(UnitTarget &t);
  virtual void onEnter();
  virtual void onUpdate();
  virtual void onExit();
};

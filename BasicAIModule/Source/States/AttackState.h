/*
 *  AttackState.h
 */
#pragma once
#include "State.h"

class AttackState: public Agent<BWAPI::Unit>
{
public:
  AttackState(Agent &a, UnitTarget &t);
  void onEnter();
  void onUpdate();
  void onExit();
};

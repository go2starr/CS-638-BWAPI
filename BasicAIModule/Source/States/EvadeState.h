/*
 *  EvadeState.h
 */
#pragma once
#include "State.h"

class EvadeState: public State
{
public:
  EvadeState(ActorAgent &a);
  void onEnter();
  void onUpdate();
  void onExit();
};

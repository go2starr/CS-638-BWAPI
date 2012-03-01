/*
 *  ScoutState.h
 */
#pragma once
#include "State.h"

class ScoutState: public State
{
public:
  ScoutState(ActorAgent &a);
  ScoutState(ActorAgent &a, PositionTarget &t);
  void onEnter();
  void onUpdate();
  void onExit();
};

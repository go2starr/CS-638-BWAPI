/* 
 *  IdleState.h
 */
#pragma once
#include "State.h"

class IdleState: public State
{
  IdleState(Agent&);            // No target
  void onEnter(Agent&, Target&);
  void onUpdate(Agent&, Target&);
  void onExit(Agent&, Target&);
};

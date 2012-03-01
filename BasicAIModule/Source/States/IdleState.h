/* 
 *  IdleState.h
 */
#pragma once
#include "State.h"

template <class Target>
class IdleState: public State<Target>
{
  void onEnter(Agent&, Target&);
  void onUpdate(Agent&, Target&);
  void onExit(Agent&, Target&);
};

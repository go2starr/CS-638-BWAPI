/* 
 *  IdleState.h
 */
#pragma once
#include "State.h"

class IdleState: public State
{
  IdleState();            // No target
  virtual void onEnter();
  virtual void onUpdate();
  virtual void onExit();
};

/*
 *  AttackState.h
 */
#pragma once
#include "State.h"

class AttackState: public Agent<BWAPI::Unit>
{

public:
  void onEnter();
  void onUpdate();
  void onExit();
};

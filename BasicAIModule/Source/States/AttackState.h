/*
 *  AttackState.h
 */
#pragma once
#include "State.h"

class AttackState: public Agent<BWAPI::Unit>
{

public:
  void onEnter(BWAPI::Unit target);
  void onUpdate(BWAPI::Unit target);
  void onExit(
};

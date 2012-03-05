/*
 *  GatherState.h
 */
#pragma once
#include "State.h"
#include "UnitTarget.h"
#include "ActorState.h"

class GatherState : public ActorState
{
public:
  GatherState();
  UnitTarget& getTarget();
};

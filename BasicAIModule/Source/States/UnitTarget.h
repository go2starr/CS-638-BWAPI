/*
 *  UnitTarget.h  - For targeting BWAPI::Units
 */
#pragma once
#include "Target.h"

class UnitTarget: public Target
{
  BWAPI::Unit target;
public:
  UnitTarget(BWAPI::Unit t):
    target(t)
  ~UnitTarget();

  BWAPI::Unit getTarget();
}



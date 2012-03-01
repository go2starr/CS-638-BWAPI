/*
 *  UnitTarget.h  - For targeting BWAPI::Units
 */
#pragma once
#include "Target.h"

class UnitTarget: public Target
{
  BWAPI::Unit &target;
public:
  UnitTarget(BWAPI::Unit &target);
  ~UnitTarget();

  BWAPI::Unit getTarget();
}



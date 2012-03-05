/*
 *  UnitTarget.h  - For targeting BWAPI::Units
 */
#pragma once
#include "Target.h"


class UnitTarget : public Target
{
  Unit &target;
public:
  UnitTarget(Unit &target);
  ~UnitTarget();

  Unit& getTarget();
};



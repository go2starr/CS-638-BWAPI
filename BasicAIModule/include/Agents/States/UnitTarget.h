/*
 *  UnitTarget.h  - For targeting BWAPI::Units
 */
#pragma once
#include "Target.h"


class UnitTarget : public Target
{
  Unit *unit;
public:
	UnitTarget(Unit *unit) { this->unit = unit; }
  ~UnitTarget();

  Unit* getUnit() { return unit; }
};



/*
 *  PositionTarget.h  - For targetting BWAPI::Position
 */
#pragma once
#include "Target.h"

class PositionTarget: public Target
{
  BWAPI::Position target;
public:
  PositionTarget(BWAPI::Position &target);
  ~PositionTarget();

  BWAPI::Position getTarget();
};

/*
 *  StructureAgent.h
 */
#pragma once
#include "Agent.h"

class StructureAgent: public Agent
{
public:
  virtual void setState(StructureState &s);
  virtual StructureState& getState();
}

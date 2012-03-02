/*
 *  AddonAgent.h
 */
#pragma once
#include "Agent.h"

class AddonAgent: public Agent
{
public:
  virtual void setState(AddonState &s);
  virtual AddonState& getState();
}

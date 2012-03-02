/*
 *  ActorAgent.h
 */
#pragma once
#include "Agent.h"

class ActorAgent: public Agent
{
public:
  virtual void setState(ActorState &s);
  virtual ActorState& getState();
};

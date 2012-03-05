/*
 *  ActorAgent.h
 */
#pragma once
#include "Agent.h"
#include "Agents/States/ActorState.h"

class ActorAgent: public Agent
{
public:
  virtual void setState(ActorState &s);
  virtual ActorState& getState();
};

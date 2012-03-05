/*
 *  ActorAgent.h
 */
#pragma once
#include "Agent.h"
#include "States/ActorState.h"

class ActorAgent : public Agent
{
protected:
  ActorAgent(Unit &u);
  virtual void setState(ActorState &s);
};

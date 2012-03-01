/*
 *  ActorAgent.h
 */
#pragma once
#include "Agent.h"

class ActorAgent: public Agent
{
public:
  // C'tor
  ActorAgent(BWAPI::Unit&);

  // D'tor
  ~ActorAgent();

  // Set State
  void setState(ActorState s);
};

/*
 *  GroundAgent.h
 */
#pragma once
#include "ActorAgent.h"

class GroundAgent : public ActorAgent
{
protected:
  GroundAgent(Unit &u);
  virtual void setState(ActorState &s);
};

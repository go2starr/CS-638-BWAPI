/*
 *  GroundAgent.h
 */
#pragma once
#include "ActorAgent.h"

class GroundAgent : public ActorAgent
{
public:
	virtual void update();
protected:
  GroundAgent(Unit &u);
};

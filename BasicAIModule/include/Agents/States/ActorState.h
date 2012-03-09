/*
 *  ActorState.h - States ActorAgents can be in
 */
#pragma once
#include "State.h"

class ActorState: public State
{
public:
	virtual StateId getId()=0;
};
  

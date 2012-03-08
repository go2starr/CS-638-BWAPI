#pragma once
/*
 * MarineAgent.h
 */
#include "GroundAgent.h"

#include <BWAPI.h>


class MarineAgent :	public GroundAgent
{
public:
	MarineAgent(BWAPI::Unit& u);
	virtual void update();
	void setState(ActorState& s);
};

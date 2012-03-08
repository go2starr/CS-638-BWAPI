#pragma once
/*
 * MedicAgent.h
 */
#include "GroundAgent.h"

#include <BWAPI.h>


class MedicAgent : public GroundAgent
{
public:
	MedicAgent(BWAPI::Unit& u);
	virtual void update();
	void setState(ActorState& s);
};
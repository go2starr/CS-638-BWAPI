/*
 *  GatherState.h
 */
#pragma once
#include "State.h"
#include "UnitTarget.h"
#include "ActorState.h"

class GatherState : public ActorState
{
public:
	GatherState();
	UnitTarget* getTarget() { return static_cast<UnitTarget*>(target); }
	void setTarget(UnitTarget *target) { this->target = target; }

	StateId getId() { return State::GatherState; }
};

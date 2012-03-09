/*
 *  DefendState.h
 */
#pragma once
#include "State.h"

class DefendState: public ActorState
{
public:
	DefendState(UnitTarget &t);
	virtual void setTarget(UnitTarget &t);
	virtual UnitTarget& getTarget();
	StateId getId() { return State::DefendState; }
};

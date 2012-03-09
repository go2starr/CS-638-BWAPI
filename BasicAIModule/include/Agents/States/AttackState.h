/*
 *  AttackState.h
 */
#pragma once
#include "State.h"

class AttackState: public ActorState
{
public:
	AttackState(UnitTarget &t);
	virtual void setTarget(UnitTarget &t);
	virtual UnitTarget& getTarget();

	StateId getId() { return State::AttackState; }
};

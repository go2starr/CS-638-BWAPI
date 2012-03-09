/* 
 *  IdleState.h
 */
#pragma once
#include "State.h"


class IdleState: public State
{
	IdleState() : State() { } 

	StateId getId() { return State::IdleState; }
};

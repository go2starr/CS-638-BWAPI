/*
 *  AddonState
 */
#pragma once
#include "State.h"

class AddonState: public State
{
public:
	StateId getId() { return State::AddonState; }
};

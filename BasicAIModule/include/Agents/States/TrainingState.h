/*
 *  TrainingState.h - State a StructureAgent is in while training a Unit */
#pragma once
#include "StructureState.h"

class TrainingState: public StructureState
{
	StateId getId() { return State::TrainingState; }
};

/*
 *  BuildingState.h - State that a StructureAgent is in while being built
 */
#pragma once
#include "StructureState.h"

class BuildingState: public StructureState
{
public:
	StateId getId() { return State::BuildingState; }
};

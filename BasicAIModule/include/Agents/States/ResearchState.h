/*
 *  ResearchState.h - State that a StructureAgent is in while researching a tech
 */
#pragma once
#include "StructureState.h"

class ResearchState: public StructureState
{

	StateId getId() { return State::ResearchState; }
};

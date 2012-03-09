/*
 *  UpgradeState.h - State that a StructureAgent is in while upgrading 
 */
#pragma once
#include "StructureState.h"

class UpgradeState: public StructureState
{
	StateId getId() { return State::UpgradeState; }
};

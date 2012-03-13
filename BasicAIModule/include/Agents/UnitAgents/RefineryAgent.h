/*
 *  RefineryAgent.h
 */
#pragma once

#include "StructureAgent.h"
#include <BWAPI.h>

class RefineryAgent : public StructureAgent
{
public:
	RefineryAgent(BWAPI::Unit& u);
	virtual void update();
};

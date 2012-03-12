/*
 *  CommandCenterAgent.h
 */
#pragma once

#include "StructureAgent.h"
#include <BWAPI.h>

class CommandCenterAgent : public StructureAgent
{
public:
	CommandCenterAgent(BWAPI::Unit& u);
	virtual void update();
};
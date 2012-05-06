/*
 * CommandCenterAgent.cpp
 */
#include "CommandCenterAgent.h"
#include "StructureAgent.h"

#include <BWAPI.h>

using namespace BWAPI;


CommandCenterAgent::CommandCenterAgent(Unit &u) 
	: StructureAgent(u)
{}

void CommandCenterAgent::update()
{
	StructureAgent::update();
}

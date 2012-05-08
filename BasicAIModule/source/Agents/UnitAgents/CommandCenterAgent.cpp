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
	// Keep workers in production, if resources available
	if (buildQueue.empty() && Broodwar->self()->minerals() > 300)
		build(UnitTypes::Terran_SCV, 1);

	StructureAgent::update();
}

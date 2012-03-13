/*
 * RefineryAgent.cpp
 */
#include "RefineryAgent.h"
#include "StructureAgent.h"

#include <BWAPI.h>

using namespace BWAPI;


RefineryAgent::RefineryAgent(Unit &u) 
	: StructureAgent(u)
{}

void RefineryAgent::update()
{
	StructureAgent::update();
}

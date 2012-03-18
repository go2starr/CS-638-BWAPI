/*
 * BarracksAgent.cpp
 */
#include "BarracksAgent.h"
#include "StructureAgent.h"

#include <BWAPI.h>
#include <BWTA.h>

using namespace BWAPI;


BarracksAgent::BarracksAgent(Unit &u) 
	: StructureAgent(u)
{}

void BarracksAgent::update()
{
	StructureAgent::update();
}

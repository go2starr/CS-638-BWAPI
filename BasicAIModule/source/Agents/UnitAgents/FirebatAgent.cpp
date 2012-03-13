/*
 * FirebatAgent.cpp
 */
#include "FirebatAgent.h"
#include "GroundAgent.h"

#include <BWAPI.h>

using BWAPI::Unit;


FirebatAgent::FirebatAgent(Unit& u)
	: GroundAgent(u)
{ }

void FirebatAgent::update()
{
	if( !unit.isIdle() )
		return;
}



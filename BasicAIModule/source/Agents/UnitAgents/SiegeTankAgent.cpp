/*
 * SiegeTankAgent.cpp
 */
#include "SiegeTankAgent.h"
#include "GroundAgent.h"

#include <BWAPI.h>

using BWAPI::Unit;


SiegeTankAgent::SiegeTankAgent(Unit& u)
	: GroundAgent(u)
{ }

void SiegeTankAgent::update()
{
	if( !unit.isIdle() )
		return;
}


/*
 * MarineAgent.cpp
 */
#include "MarineAgent.h"
#include "GroundAgent.h"

#include <BWAPI.h>

using BWAPI::Unit;


MarineAgent::MarineAgent(Unit& u)
	: GroundAgent(u)
{ }

void MarineAgent::update()
{
    GroundAgent::update();
}



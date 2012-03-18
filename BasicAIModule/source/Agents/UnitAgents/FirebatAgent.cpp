/*
 * FirebatAgent.cpp
 */
#include "FirebatAgent.h"
#include "GroundAgent.h"

#include <BWAPI.h>

using BWAPI::Unit;


FirebatAgent::FirebatAgent(Unit& u)
	: GroundAgent(u)
{
//    state = DefendState;
}

void FirebatAgent::update()
{
    GroundAgent::update();
}



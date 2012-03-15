/*
 * MedicAgent.cpp
 */
#include "MedicAgent.h"
#include "GroundAgent.h"

#include <BWAPI.h>

using BWAPI::Unit;


MedicAgent::MedicAgent(Unit& u)
	: GroundAgent(u)
{ }

void MedicAgent::update()
{
    GroundAgent::update();
}
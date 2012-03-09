/*
 * SCVAgent.cpp
 */
#include "UnitAgents/SCVAgent.h"
#include "GroundAgent.h"

#include <BWAPI.h>

using BWAPI::Unit;


SCVAgent::SCVAgent(Unit &u) 
	: GroundAgent(u)
{}

void SCVAgent::update()
{
	if (state) {
		switch (state) {
			case GatherState:
				break;

			default:
				GroundAgent::update();
		}
	}
}

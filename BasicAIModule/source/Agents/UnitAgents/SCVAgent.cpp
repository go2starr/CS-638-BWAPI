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
		GatherState *gatherState;

		switch (state->getId()) {
			case State::GatherState:
				gatherState = static_cast<GatherState*>(state);
				unit.gather(gatherState->getTarget()->getUnit());
				break;

			default:
				GroundAgent::update();
		}
	}
}

void SCVAgent::setState(ActorState &s) 
{
	state = &s;
}

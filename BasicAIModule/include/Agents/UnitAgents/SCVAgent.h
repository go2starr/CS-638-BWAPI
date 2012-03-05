/*
 *
 */
#pragma once
#include "../GroundAgent.h"

class SCVAgent : public GroundAgent
{
public:
	SCVAgent(Unit &u);
	void setState(ActorState &s);
};

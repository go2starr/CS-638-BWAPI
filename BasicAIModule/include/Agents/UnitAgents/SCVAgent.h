/*
 *
 */
#pragma once
#include "../GroundAgent.h"

class SCVAgent : public GroundAgent
{
public:
	SCVAgent(Unit &u);
	virtual void update();
	void setState(ActorState &s);
};

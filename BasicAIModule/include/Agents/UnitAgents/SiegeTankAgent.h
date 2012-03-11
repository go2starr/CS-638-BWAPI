#pragma once
/*
 * SiegeTankAgent.h
 */
#include "GroundAgent.h"

#include <BWAPI.h>


class SiegeTankAgent : public GroundAgent
{
public:
	SiegeTankAgent(BWAPI::Unit& u);
	virtual void update();
};

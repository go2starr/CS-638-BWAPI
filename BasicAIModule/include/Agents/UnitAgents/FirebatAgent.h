#pragma once
/*
 * FirebatAgent.h
 */
#include "GroundAgent.h"

#include <BWAPI.h>


class FirebatAgent : public GroundAgent
{
public:
	FirebatAgent(BWAPI::Unit& u);
	virtual void update();
};

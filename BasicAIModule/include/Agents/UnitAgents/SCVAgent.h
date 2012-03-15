#pragma once
/*
 * SCVAgent.h
 */
#include "GroundAgent.h"

#include <BWAPI.h>


class SCVAgent : public GroundAgent
{
public:
	SCVAgent(BWAPI::Unit& u);
	virtual void update();

private:
	BWAPI::TilePosition buildLocation;
	bool wasConstructing;
	// what we are building
	BWAPI::Unit * constructingStructure;
};

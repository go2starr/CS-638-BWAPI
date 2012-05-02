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

	// gatherMinerals - make this SCV agent mine minerals.  Returns 0 on failure.
	bool gatherMinerals();

private:
	BWAPI::TilePosition buildLocation;
	bool wasConstructing;
	// what we are building
	BWAPI::Unit * constructingStructure;
};

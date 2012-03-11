#pragma once
/*
 * TacticalBuildingPlacer.h
 */
#include <BWAPI.h>
#include <BWTA.h>

#include <EnhancedUI.h>
#include <EnhancedSide.h>
#include <EnhancedChokepoint.h>

#include <assert.h>

using std::pair;
using std::vector;


class TacticalBuildingPlacer
{
public:
	TacticalBuildingPlacer();
	~TacticalBuildingPlacer(void);
	void analyze(void);
	void draw(void);
	void update(void);

private:
	vector<pair<BWTA::Region *, vector<EnhancedChokepoint>>> regionsToChokepoints;
};

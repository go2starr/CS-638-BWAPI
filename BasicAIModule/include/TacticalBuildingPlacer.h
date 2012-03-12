#pragma once
/*
 * TacticalBuildingPlacer.h
 */
#include <BWAPI.h>
#include <BWTA.h>
#include <BWSAL.h>

#include <EnhancedUI.h>
#include <EnhancedSide.h>
#include <EnhancedChokepoint.h>

#include <assert.h>

using std::pair;
using std::vector;


class TacticalBuildingPlacer
{
public:
	// TacticalBuildingPlacer is a singleton
	static TacticalBuildingPlacer& instance() { static TacticalBuildingPlacer s; return s; }
	~TacticalBuildingPlacer(void);

	void analyze(void);
	void draw(void);
	void update(void);
	BWAPI::TilePosition reserveBuildLocation(BWAPI::UnitType unitType, BWAPI::TilePosition seedLocation, BWAPI::Unit *builder = NULL);

private:
	TacticalBuildingPlacer();

	vector<pair<BWTA::Region *, vector<EnhancedChokepoint>>> regionsToChokepoints;
	BWSAL::BFSBuildingPlacer buildingPlacer;
};

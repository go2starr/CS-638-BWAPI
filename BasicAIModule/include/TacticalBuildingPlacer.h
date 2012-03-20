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
using std::set;

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
	// search for optimal build pattern to wall off chokepoint in a region
	vector<BWAPI::TilePosition> chokepointBuildPatternSearch(EnhancedChokepoint ecpoint, BWTA::Region * region);

	vector<pair<BWTA::Region *, vector<EnhancedChokepoint>>> regionsToChokepoints;
	// for chokepointBuildPatternSearch() return
	vector<vector<BWAPI::TilePosition>> chokepointBuildLocations;
	BWSAL::BFSBuildingPlacer buildingPlacer;
};

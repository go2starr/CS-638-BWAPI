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
	// chokepointBuildPatternSearch() helper
	pair<int, int> sideEndsMinGap(bool horizontalGrowth, pair<BWAPI::TilePosition, BWAPI::TilePosition> endTiles);
	int facingSideMinGap(EnhancedSide::Orientation orientation, BWAPI::TilePosition facingSideEnd);
	// chokepointBuildPatternSearch() helper
	int sideCoverage(EnhancedSide * ecpSide, pair<BWAPI::TilePosition, BWAPI::TilePosition> endTiles, 
		vector<EnhancedSide> * currentSidesNotCovered = NULL);
	// chokepointBuildPatternSearch() helper
	// attempts to grow out endTiles in specified directions sideGrowDirs starting at baseTile
	// stops growing if tiles are unbuildable or if chokepoint side coverage doesn't increase
	// returns length that still isn't covered after growing
	// endTiles should already be initialized, and should be anyways to get initialLengthNotCovered
	int growEndsByCoverage(EnhancedSide * ecpSide, pair<BWAPI::TilePosition, BWAPI::TilePosition> & endTiles, 
		BWAPI::TilePosition baseTile, int initialLengthNotCovered, pair<pair<int,int>,pair<int,int>> sideGrowDirs);
	// returns true if either side has poor gap
	bool growEndsByGap(EnhancedSide::Orientation orientation, pair<BWAPI::TilePosition, BWAPI::TilePosition> & endTiles, 
		BWAPI::TilePosition baseTile, pair<pair<int,int>,pair<int,int>> sideGrowDirs, int & tileFacingGapSideA, int & tileFacingGapSideB);
	// used after other grow functions when its known this 
	// is the right direction to be growing
	// doesn't start at the baseTile, but picks up after the build tiles
	// returs the over all length
	int growEndsByLength(pair<BWAPI::TilePosition, BWAPI::TilePosition> & endTiles, 
		pair<pair<int,int>,pair<int,int>> sideGrowDirs);
	bool growEndsByBumping(bool horizontalGrowth, pair<BWAPI::TilePosition, BWAPI::TilePosition> & endTiles, 
											 pair<pair<int,int>,pair<int,int>> sideGrowDirs, int & minTileGapSideA,
											 int & minTileGapSideB);

	// each region will have a number of chokepoints
	vector<pair<BWTA::Region *, vector<EnhancedChokepoint>>> regionsToChokepoints;
	// for chokepointBuildPatternSearch() return
	vector<vector<BWAPI::TilePosition>> chokepointBuildLocations;
	BWSAL::BFSBuildingPlacer buildingPlacer;
};

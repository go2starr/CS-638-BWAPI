#pragma once
/*
* TacticalBuildingPlacer.cpp
*/
#include "TacticalBuildingPlacer.h"
#include "DebugFlags.h"
#include "Common.h"

#include <BWAPI.h>
#include <BWSAL/BFSBuildingPlacer.h>

#include <algorithm>

using namespace BWAPI;
using namespace BWTA;


TacticalBuildingPlacer::TacticalBuildingPlacer()
{

	BWTA::BaseLocation * baseLoc;
	BWTA::Region * region;
	set<BWTA::Chokepoint *> startLocChokepoints;
	set<BWTA::Chokepoint *>::iterator cpi;
	pair<BWTA::Region *, vector<EnhancedChokepoint>> regionChokepoints;

	// get enhanced chokepoints for start location
	baseLoc = BWTA::getStartLocation(BWAPI::Broodwar->self());
	region = baseLoc->getRegion();
	startLocChokepoints = region->getChokepoints();

	// set base region 
	regionChokepoints.first = region;

	// set chokepoints for base region
	for (cpi = startLocChokepoints.begin(); cpi != startLocChokepoints.end(); ++cpi) {
		EnhancedChokepoint ecPoint(*cpi);
		regionChokepoints.second.push_back(ecPoint);
	}

	// save combo 
	regionsToChokepoints.push_back(regionChokepoints);

	// search for build patterns for base region chokepoints
	for (int m = 0; m < (int) regionChokepoints.second.size(); ++m) {
		vector<BWAPI::TilePosition> cpBuildLocation;
		cpBuildLocation = chokepointBuildPatternSearch(regionChokepoints.second[m], regionChokepoints.first);
		// save
		chokepointBuildLocations.push_back(cpBuildLocation);
	}
}

TacticalBuildingPlacer::~TacticalBuildingPlacer(void)
{
}

int TacticalBuildingPlacer::facingSideMinGap(EnhancedSide::Orientation orientation, BWAPI::TilePosition facingSideEnd)
{

	pair<BWAPI::Position, BWAPI::Position> sideEndPoints;
	int facingSideEndTileGap;
	int facingSideEndWidth, facingSideEndHeight;
	int xStartPos, yStartPos;
	int xEndPos, yEndPos;
	const BWAPI::UnitType & depot = BWAPI::UnitTypes::Terran_Supply_Depot;
	EnhancedSide * facingSide;

	facingSideEndWidth = (depot.tileWidth() * 32) - 1;
	facingSideEndHeight = (depot.tileHeight() * 32) - 1;
	xStartPos = facingSideEnd.x() * 32;
	yStartPos = facingSideEnd.y() * 32;
	xEndPos = xStartPos + facingSideEndWidth;
	yEndPos = yStartPos + facingSideEndHeight;

	if (orientation == EnhancedSide::left) {
		sideEndPoints.first.x() = xStartPos;
		sideEndPoints.first.y() = yStartPos;
		sideEndPoints.second.x() = xStartPos;
		sideEndPoints.second.y() = yEndPos;
	}
	else if (orientation == EnhancedSide::top) {
		sideEndPoints.first.x() = xStartPos;
		sideEndPoints.first.y() = yStartPos;
		sideEndPoints.second.x() = xEndPos;
		sideEndPoints.second.y() = yStartPos;
	}
	else if (orientation == EnhancedSide::right) {
		sideEndPoints.first.x() = xEndPos;
		sideEndPoints.first.y() = yStartPos;
		sideEndPoints.second.x() = xEndPos;
		sideEndPoints.second.y() = yEndPos;		
	}
	else if (orientation == EnhancedSide::bottom) {
		sideEndPoints.first.x() = xStartPos;
		sideEndPoints.first.y() = yEndPos;
		sideEndPoints.second.x() = xEndPos;
		sideEndPoints.second.y() = yEndPos;
	}

	facingSide = new EnhancedSide(sideEndPoints, orientation);

	facingSideEndTileGap = facingSide->checkMinGap();
	delete(facingSide);
	return facingSideEndTileGap;
}

// if horizontal growth, uses the 2 vertical sides from endTiles
// if vertical growth, usese the 2 horizontal sides from endTiles
pair<int, int> TacticalBuildingPlacer::sideEndsMinGap(bool horizontalGrowth, pair<BWAPI::TilePosition, BWAPI::TilePosition> endTiles)
{

	pair <int, int> sideEndGaps;
	pair<BWAPI::Position, BWAPI::Position> sideEndPoints;
	int currentTileGapSideA, currentTileGapSideB;
	int buildWidth, buildHeight;
	const BWAPI::UnitType & depot = BWAPI::UnitTypes::Terran_Supply_Depot;

	buildWidth = depot.tileWidth();
	buildHeight = depot.tileHeight();

	if (horizontalGrowth) {
		// check side A gap
		sideEndPoints.first.x() = endTiles.first.x() * 32;
		sideEndPoints.first.y() = endTiles.first.y() * 32;
		sideEndPoints.second.x() = sideEndPoints.first.x(); // same
		sideEndPoints.second.y() = sideEndPoints.first.y() + (buildHeight * 32) - 1;
		EnhancedSide buildSideA(sideEndPoints, EnhancedSide::left);
		currentTileGapSideA = buildSideA.checkMinGap();

		// check side B gap
		sideEndPoints.first.x() = ((endTiles.second.x() + buildWidth) * 32) - 1;
		// sideEndPoints.first.y() same
		sideEndPoints.second.x() = sideEndPoints.first.x(); // same
		// sideEndPoints.second.y() same
		EnhancedSide buildSideB(sideEndPoints, EnhancedSide::right);
		currentTileGapSideB = buildSideB.checkMinGap();
	}
	else {
		// check side A gap
		sideEndPoints.first.x() = endTiles.first.x() * 32;
		sideEndPoints.first.y() = endTiles.first.y() * 32;
		sideEndPoints.second.x() = sideEndPoints.first.x() + (buildWidth * 32) - 1;
		sideEndPoints.second.y() = sideEndPoints.first.y(); // same
		EnhancedSide buildSideA(sideEndPoints, EnhancedSide::top);
		currentTileGapSideA = buildSideA.checkMinGap();

		// check side B gap
		// sideEndPoints.first.x() same
		sideEndPoints.first.y() = ((endTiles.second + buildHeight) * 32) - 1;
		// sideEndPoints.second.x() same
		sideEndPoints.second.y() = sideEndPoints.first.y(); // same
		EnhancedSide buildSideB(sideEndPoints, EnhancedSide::bottom);
		currentTileGapSideB = buildSideB.checkMinGap();
	}
	sideEndGaps.first = currentTileGapSideA;
	sideEndGaps.second = currentTileGapSideB;

	return sideEndGaps;
}


// make sure to clear currentSidesNotCovered before hand if need be
// currentSidesNotCovered default is NULL
int TacticalBuildingPlacer::sideCoverage(EnhancedSide * ecpSide, pair<BWAPI::TilePosition, BWAPI::TilePosition> endTiles, 
										 vector<EnhancedSide> * currentSidesNotCovered)
{
	pair<BWAPI::Position, BWAPI::Position> sideEndPoints;
	int buildWidth, buildHeight;
	const BWAPI::UnitType & depot = BWAPI::UnitTypes::Terran_Supply_Depot;
	int currentLengthNotCovered;

	buildWidth = depot.tileWidth();
	buildHeight = depot.tileHeight();

	if (ecpSide->isHorizontal()) {
		//  -------------
		//A |   |   |   | B
		// left to right, get top side
		// get from endTiles, far left
		sideEndPoints.first.x() = endTiles.first.x() * 32;
		sideEndPoints.first.y() = endTiles.first.y() * 32;
		// get from endTiles, far right
		sideEndPoints.second.x() = ((endTiles.second.x() + buildWidth) * 32) - 1;
		sideEndPoints.second.y() = sideEndPoints.first.y(); // same
		// check coverage
		EnhancedSide coveringSide(sideEndPoints, EnhancedSide::top);
		currentLengthNotCovered = ecpSide->checkCoverage(coveringSide, currentSidesNotCovered);
	}
	else {
		// top to bottom
		// get left side and compare for coverage
		// endTile A (first) is top
		sideEndPoints.first.x() = endTiles.first.x() * 32;
		sideEndPoints.first.y() = endTiles.first.y() * 32;
		// get from endTiles, far bottom
		sideEndPoints.second.x() = sideEndPoints.first.x(); // same
		sideEndPoints.second.y() = ((endTiles.second.y() + buildHeight) * 32) - 1;
		// check coverage
		EnhancedSide coveringSide(sideEndPoints, EnhancedSide::left);
		currentLengthNotCovered = ecpSide->checkCoverage(coveringSide, currentSidesNotCovered);
	}
	return currentLengthNotCovered;
}


// returns true if either side has poor gap
// should this start at the baseTile? -- this is usually used after growEndsByCoverage,
// which may have already made some progress
// OR at least give it an option to just use the endTiles
// based off of facing side gap
bool TacticalBuildingPlacer::growEndsByGap(EnhancedSide::Orientation orientation, pair<BWAPI::TilePosition, BWAPI::TilePosition> & endTiles, 
										   BWAPI::TilePosition baseTile, pair<pair<int,int>,pair<int,int>> sideGrowDirs, int & tileFacingGapSideA, int & tileFacingGapSideB)
{

	BWAPI::TilePosition currentTile;
	int prevGapEndA, prevGapEndB;
	int currentGapEndA, currentGapEndB;
	int xGrowDirEndA, yGrowDirEndA;
	int xGrowDirEndB, yGrowDirEndB;
	int buildWidth, buildHeight;
	const BWAPI::UnitType & depot = BWAPI::UnitTypes::Terran_Supply_Depot;
	bool canBuild;
	bool poorGapA, poorGapB;

	buildWidth = depot.tileWidth();
	buildHeight = depot.tileHeight();

	currentTile = baseTile;
	canBuild = true;

	// directions
	xGrowDirEndA = sideGrowDirs.first.first;
	yGrowDirEndA = sideGrowDirs.first.second;
	xGrowDirEndB = sideGrowDirs.second.first;
	yGrowDirEndB = sideGrowDirs.second.second;

	// for finding baseline gaps
	prevGapEndA = prevGapEndB = -1;
	poorGapA = poorGapB = false;

	// init for return
	currentGapEndA = facingSideMinGap(orientation, endTiles.first);
	currentGapEndB = facingSideMinGap(orientation, endTiles.second);

	// grow end A
	while (canBuild) {
		currentTile.x() += xGrowDirEndA * buildWidth;
		currentTile.y() += yGrowDirEndA * buildHeight;
		if (BWAPI::Broodwar->canBuildHere(NULL, currentTile, BWAPI::UnitTypes::Terran_Supply_Depot, false)) {
			endTiles.first = currentTile;
			// check gap for forward progress
			if (prevGapEndA < 0) {
				prevGapEndA = facingSideMinGap(orientation, endTiles.first);
				if (prevGapEndA == 0) {
					break;
				}
			}
			else {
				currentGapEndA = facingSideMinGap(orientation, endTiles.first);
				if (currentGapEndA >= prevGapEndA) {
					poorGapA = true;
					break;
				}
				else if (currentGapEndA == 0) {
					break;
				}
			}
		}
		else {
			canBuild = false;
			break;
		}
	}
	// grow end B
	currentTile = baseTile;
	canBuild = true;

	while (canBuild) {
		currentTile.x() += xGrowDirEndB * buildWidth;
		currentTile.y() += yGrowDirEndB * buildHeight;

		if (BWAPI::Broodwar->canBuildHere(NULL, currentTile, BWAPI::UnitTypes::Terran_Supply_Depot, false)) {
			endTiles.second = currentTile;
			// check gap for forward progress
			if (prevGapEndB < 0) {
				prevGapEndB = facingSideMinGap(orientation, endTiles.second);
				if (prevGapEndB == 0) {
					break;
				}
			}
			else {
				currentGapEndB = facingSideMinGap(orientation, endTiles.second);
				if (currentGapEndB >= prevGapEndB) {
					poorGapB = true;
					break;
				}
				else if (currentGapEndB == 0) {
					break;
				}
			}
		}
		else {
			canBuild = false;
			break;
		}
	}

	// set gaps
	tileFacingGapSideA = currentGapEndA;
	tileFacingGapSideB = currentGapEndB;

	return (poorGapA || poorGapB);
}

// attempts to grow out endTiles in specified directions sideGrowDirs starting at baseTile
// stops growing if tiles are unbuildable or if chokepoint side coverage doesn't increase
// returns length that still isn't covered after growing
// endTiles should already be initialized, and should be anyways to get initialLengthNotCovered
int TacticalBuildingPlacer::growEndsByCoverage(EnhancedSide * ecpSide, pair<BWAPI::TilePosition, BWAPI::TilePosition> & endTiles, 
											   BWAPI::TilePosition baseTile, int initialLengthNotCovered, pair<pair<int,int>,pair<int,int>> sideGrowDirs)
{

	BWAPI::TilePosition currentTile;
	int prevLengthNotCovered, currentLengthNotCovered;
	int xGrowDirEndA, yGrowDirEndA;
	int xGrowDirEndB, yGrowDirEndB;
	int buildWidth, buildHeight;
	const BWAPI::UnitType & depot = BWAPI::UnitTypes::Terran_Supply_Depot;
	bool canBuild;
	// get initial coverage first (baseline)
	// 	minLengthNotCovered = currentLengthNotCovered;

	buildWidth = depot.tileWidth();
	buildHeight = depot.tileHeight();

	prevLengthNotCovered = initialLengthNotCovered;
	// in case nothing can be built
	currentLengthNotCovered = initialLengthNotCovered;
	currentTile = baseTile;
	canBuild = true;

	// directions
	xGrowDirEndA = sideGrowDirs.first.first;
	yGrowDirEndA = sideGrowDirs.first.second;
	xGrowDirEndB = sideGrowDirs.second.first;
	yGrowDirEndB = sideGrowDirs.second.second;

	// grow end A
	while (canBuild) {
		currentTile.x() += xGrowDirEndA * buildWidth;
		currentTile.y() += yGrowDirEndA * buildHeight;
		if (BWAPI::Broodwar->canBuildHere(NULL, currentTile, BWAPI::UnitTypes::Terran_Supply_Depot, false)) {
			endTiles.first = currentTile;
			// check coverage for forward progress
			currentLengthNotCovered = sideCoverage(ecpSide, endTiles);
			if (currentLengthNotCovered == 0) {
				break;
			}
			if (currentLengthNotCovered >= prevLengthNotCovered) {
				// stop growing
				break;
			}
			else {
				prevLengthNotCovered = currentLengthNotCovered;
			}
		}
		else {
			canBuild = false;
			break;
		}
	}
	// grow end B
	currentTile = baseTile;
	canBuild = true;

	while (canBuild) {
		currentTile.x() += xGrowDirEndB * buildWidth;
		currentTile.y() += yGrowDirEndB * buildHeight;

		if (BWAPI::Broodwar->canBuildHere(NULL, currentTile, BWAPI::UnitTypes::Terran_Supply_Depot, false)) {
			endTiles.second = currentTile;
			// check coverage for forward progress
			currentLengthNotCovered = sideCoverage(ecpSide, endTiles);
			if (currentLengthNotCovered == 0) {
				break;
			}
			if (currentLengthNotCovered >= prevLengthNotCovered) {
				// stop growing
				break;
			}
			else {
				prevLengthNotCovered = currentLengthNotCovered;
			}
		}
		else {
			canBuild = false;
			break;
		}
	}

	return currentLengthNotCovered;
}

// used after other grow functions when its known this 
// is the right direction to be growing
// doesn't start at the baseTile, but picks up after the build tiles
// returs the over all length
int TacticalBuildingPlacer::growEndsByLength(pair<BWAPI::TilePosition, BWAPI::TilePosition> & endTiles, 
											 pair<pair<int,int>,pair<int,int>> sideGrowDirs)
{

	BWAPI::TilePosition currentTile;
	int xGrowDirEndA, yGrowDirEndA;
	int xGrowDirEndB, yGrowDirEndB;
	int buildWidth, buildHeight;
	const BWAPI::UnitType & depot = BWAPI::UnitTypes::Terran_Supply_Depot;
	bool canBuild;
	int numBuildTiles;
	bool xShared, yShared, rightOrder;

	numBuildTiles = 0;
	buildWidth = depot.tileWidth();
	buildHeight = depot.tileHeight();

	currentTile = endTiles.first;
	canBuild = true;
	xShared = yShared = rightOrder = false;

	// directions
	xGrowDirEndA = sideGrowDirs.first.first;
	yGrowDirEndA = sideGrowDirs.first.second;
	xGrowDirEndB = sideGrowDirs.second.first;
	yGrowDirEndB = sideGrowDirs.second.second;

	// grow end A
	while (canBuild) {
		currentTile.x() += xGrowDirEndA * buildWidth;
		currentTile.y() += yGrowDirEndA * buildHeight;
		if (BWAPI::Broodwar->canBuildHere(NULL, currentTile, BWAPI::UnitTypes::Terran_Supply_Depot, false)) {
			endTiles.first = currentTile;
		}
		else {
			canBuild = false;
			break;
		}
	}
	// grow end B
	currentTile = endTiles.second;
	canBuild = true;

	while (canBuild) {
		currentTile.x() += xGrowDirEndB * buildWidth;
		currentTile.y() += yGrowDirEndB * buildHeight;

		if (BWAPI::Broodwar->canBuildHere(NULL, currentTile, BWAPI::UnitTypes::Terran_Supply_Depot, false)) {
			endTiles.second = currentTile;
		}
		else {
			canBuild = false;
			break;
		}
	}

	// ensure endTiles share a coord and that the
	// first coord is less than the 2nd

	// find shared coord
	if (endTiles.first.x() == endTiles.second.x()) {
		xShared = true;
	}
	else {
		yShared = true;
	}

	if (xShared || yShared) {
		if ((endTiles.first.x() <= endTiles.second.x()) || 
			(endTiles.first.y() <= endTiles.second.y())) {
				rightOrder = true;
		}
	}

	if (rightOrder) {
		currentTile = endTiles.first;
		while (currentTile != endTiles.second) {
			currentTile.x() += xGrowDirEndB * buildWidth;
			currentTile.y() += yGrowDirEndB * buildHeight;
			numBuildTiles++;
			// check for overrun
			if (numBuildTiles > 10) {
				break;
			}
		}
	}

	return numBuildTiles;
}

// check to see if bumping the set by 1 or 2 tiles 
// will open up another build position and close the gap
// if horizontal, look for gap total of 3
// if vertical, look for gap total of 2 (1 on either side)
// set min gap for sides if an improvement
// return true if able to bump grow
bool TacticalBuildingPlacer::growEndsByBumping(bool horizontalGrowth, pair<BWAPI::TilePosition, BWAPI::TilePosition> & endTiles, 
											   pair<pair<int,int>,pair<int,int>> sideGrowDirs, int & minTileGapSideA,
											   int & minTileGapSideB)
{

	BWAPI::TilePosition currentTile;
	int xGrowDirEndA, yGrowDirEndA;
	int xGrowDirEndB, yGrowDirEndB;
	int currentTileGapSideA, currentTileGapSideB;
	pair<int, int> sideEndGaps;
	int buildWidth, buildHeight;
	const BWAPI::UnitType & depot = BWAPI::UnitTypes::Terran_Supply_Depot;
	bool canBuild;
	bool madeImprovement;

	buildWidth = depot.tileWidth();
	buildHeight = depot.tileHeight();

	canBuild = true;
	madeImprovement = false;

	// directions
	xGrowDirEndA = sideGrowDirs.first.first;
	yGrowDirEndA = sideGrowDirs.first.second;
	xGrowDirEndB = sideGrowDirs.second.first;
	yGrowDirEndB = sideGrowDirs.second.second;


	// check for propper gap first

	// leave grow here for now, but should be replaced with growEnds()
	// FIX as gap is now in walkable tiles, NOT build tiles -- done
	if ((horizontalGrowth && (minTileGapSideA / 4) + (minTileGapSideB / 4) == buildWidth) ||
		(!horizontalGrowth && (minTileGapSideA / 4) + (minTileGapSideB / 4) == buildHeight)) {
			// shift first side 1 or 2 tiles and try to grow the second side
			if (horizontalGrowth) {
				currentTile.x() = endTiles.first.x() - (minTileGapSideA / 4);
			}
			else {
				currentTile.y() = endTiles.first.y() - (minTileGapSideA / 4);
			}

			if (BWAPI::Broodwar->canBuildHere(NULL, currentTile, BWAPI::UnitTypes::Terran_Supply_Depot, false)) {
				endTiles.first = currentTile;
				// keep going and grow end B
				canBuild = true;

				while (canBuild) {
					currentTile.x() += xGrowDirEndB * buildWidth;
					currentTile.y() += yGrowDirEndB * buildHeight;

					if (BWAPI::Broodwar->canBuildHere(NULL, currentTile, BWAPI::UnitTypes::Terran_Supply_Depot, false)) {
						endTiles.second = currentTile;
					}
					else {
						canBuild = false;
						break;
					}
				}
			}

			// check for gap improvement after bumping

			sideEndGaps = sideEndsMinGap(horizontalGrowth, endTiles);
			currentTileGapSideA = sideEndGaps.first;
			currentTileGapSideB = sideEndGaps.second;

			if (currentTileGapSideA + currentTileGapSideB < minTileGapSideA + minTileGapSideB) {
				madeImprovement = true;
				minTileGapSideA = currentTileGapSideA;
				minTileGapSideB = currentTileGapSideB;
			}
	} // end bump check

	return madeImprovement;
}


// search for optimal build pattern to wall off chokepoint in a region
// search is off the base tile, ie baseTile = ecpoint.getBuildTile(region);
// so for a given enhanced chokepoint and a region, use the base tile
// as a starting point to search from
vector<BWAPI::TilePosition> TacticalBuildingPlacer::chokepointBuildPatternSearch(
	EnhancedChokepoint ecpoint, BWTA::Region * region) 
{
	vector<BWAPI::TilePosition> buildTiles;
	//vector<BWAPI::TilePosition> currentBuildTiles;
	// side A and B
	pair<BWAPI::TilePosition, BWAPI::TilePosition> endTiles, prevEndTiles;
	// main tile to search from
	BWAPI::TilePosition baseTile;
	BWAPI::TilePosition currentTile;
	int buildWidth, buildHeight;
	const BWAPI::UnitType & depot = BWAPI::UnitTypes::Terran_Supply_Depot;
	vector<EnhancedSide> ecpSides;
	vector<EnhancedSide> currentSidesNotCovered;
	EnhancedSide * ecpSide;
	pair<BWAPI::Position, BWAPI::Position> sideEndPoints;
	// pixels
	int minLengthNotCovered;
	int currentLengthNotCovered;
	// gap in walk tiles (8x8 px)
	pair <int, int> sideEndGaps;
	int minTileGapSideA, minTileGapSideB;
	int minTileFacingGapSideA, minTileFacingGapSideB;
	int maxTileGapSideA, maxTileGapSideB;
	int currentTileGapSideA, currentTileGapSideB;
	int currentTileFacingGapSideA, currentTileFacingGapSideB;
	int prevTileGapSideA, prevTileGapSideB;
	int prevTileFacingGapSideA, prevTileFacingGapSideB;
	int ecpNumSides;
	EnhancedSide::Orientation orientation, facingSideOrientation;
	pair<pair<int,int>,pair<int,int>> sideGrowDirs;
	// to extend buildings to the wall
	int xGrowDirEndA, yGrowDirEndA;
	int xGrowDirEndB, yGrowDirEndB;
	// to search for a better fit in a larger space
	int xShiftDir, yShiftDir;
	// if the end gaps together equal a building
	// same as grow dir
	// bump by the gap number
	//int xBumpDir, yBumpDir;
	bool canBuild;
	bool poorCoverage;
	bool poorFacingSideGap;
	bool poorSideEndGap;
	bool tryShifting;
	bool reverseGrowDirection;
	bool reverseShiftDirection;
	bool maxGrow;
	// count growth
	//int buildCountSideA, buildCountSideB;
	// general purpose
	int count;
	int numBuildTiles;

	assert(region);

	// init bools
	canBuild = true;
	poorCoverage = false;
	poorFacingSideGap = false;
	poorSideEndGap = false;
	tryShifting = false;
	reverseGrowDirection = false;
	reverseShiftDirection = false;
	maxGrow = false;

	baseTile = ecpoint.getBuildTile(region);
	ecpSides = ecpoint.getBoundingBoxSidesForRegion(region);
	buildWidth = depot.tileWidth();
	buildHeight = depot.tileHeight();
	ecpNumSides = (int)ecpSides.size();
	numBuildTiles = 0;


	// only handles 2 sides
	assert(ecpNumSides && ecpNumSides <= 2);

	// if sides, search on longest otherwise
	// if the 2 sides are equal in length (very common)
	// use checkMaxGap() to find the greatest gap
	// if the side is facing the whole then it will have
	// a very large max gap, this is the side we want
	// as the other side is facing the wall, ah the dunce side
	// BROKEN!! -- max gap needs to use walkable tiles instead of
	// buildable tiles, because this is giving the wrong side
	// otherwise go back to using the build tile, then the
	// 2 matching sides with greates max gap then pick same
	// vertical or horizontal side
	if (ecpNumSides > 1) {
		int size = ecpSides[0].getTilePositions().size();
		int sizeTemp;
		ecpSide = &(ecpSides[0]);
		for (int m = 1; m < ecpNumSides; ++m) {
			if ((sizeTemp = (int)ecpSides[m].getTilePositions().size()) > size) {
				size = sizeTemp;
				ecpSide = &(ecpSides[m]);
			}
			else if (sizeTemp == size) {
				// pick whichever side has the max gap
				// as we want to wall off that one

				maxTileGapSideA = ecpSides[0].checkMaxGap();
				maxTileGapSideB = ecpSides[1].checkMaxGap();

				if (maxTileGapSideA > maxTileGapSideB) {
					ecpSide = &(ecpSides[0]);
				}
				else {
					ecpSide = &(ecpSides[1]);
				}

				// gap analysis for 4 sides of the build area from baseTile
				// if left (A) and right have min of max, then pick horizontal side
				// if top (A) and bottom have min of max, then pick vertical side
				// left side
				//sideEndPoints.first.x() = baseTile.x() * 32;
				//sideEndPoints.first.y() = baseTile.y() * 32;
				//sideEndPoints.second.x() = sideEndPoints.first.x();
				//sideEndPoints.second.y() = ((baseTile.y() + buildHeight) * 32) - 1;
				//EnhancedSide left(sideEndPoints, EnhancedSide::left);
				//maxTileGapSideA = left.checkMaxGap();
				//// right side
				//sideEndPoints.first.x() = ((baseTile.x() + buildWidth) * 32) - 1;
				//sideEndPoints.first.y() = baseTile.y() * 32;
				//sideEndPoints.second.x() = sideEndPoints.first.x();
				//sideEndPoints.second.y() = ((baseTile.y() + buildHeight) * 32) - 1;
				//EnhancedSide right(sideEndPoints, EnhancedSide::right);
				//maxTileGapSideB = right.checkMaxGap();
				//// top side
				//sideEndPoints.first.x() = baseTile.x() * 32;
				//sideEndPoints.first.y() = baseTile.y() * 32;
				//sideEndPoints.second.x() = ((baseTile.x() + buildWidth) * 32) - 1;
				//sideEndPoints.second.y() = sideEndPoints.first.y();
				//EnhancedSide top(sideEndPoints, EnhancedSide::top);
				//currentTileGapSideA = top.checkMaxGap();
				//// bottom side
				//sideEndPoints.first.x() = baseTile.x() * 32;
				//sideEndPoints.first.y() = ((baseTile.y() + buildHeight) * 32) - 1;
				//sideEndPoints.second.x() = ((baseTile.x() + buildWidth) * 32) - 1;
				//sideEndPoints.second.y() = sideEndPoints.first.y();
				//EnhancedSide bottom(sideEndPoints, EnhancedSide::bottom);
				//currentTileGapSideB = bottom.checkMaxGap();
				//// maxTileGapSide is vertical
				//// currentTileGapSide is horizontal
				//// pick horizontal side
				//if (maxTileGapSideA + maxTileGapSideB > currentTileGapSideA + currentTileGapSideB) {
				//	if (ecpSides[0].isHorizontal()) {
				//		ecpSide = &(ecpSides[0]);
				//	}
				//	else {
				//		ecpSide = &(ecpSides[1]);
				//	}
				//}
				//// pick vertical side
				//else {
				//	if (!ecpSides[0].isHorizontal()) {
				//		ecpSide = &(ecpSides[0]);
				//	}
				//	else {
				//		ecpSide = &(ecpSides[1]);
				//	}
				//}
			}
		}
	}
	else {
		ecpSide = &(ecpSides[0]);
	}

	// get direction from orientation
	// grow by buildWidth or buildHeight
	// shift by one tile
	orientation = ecpSide->getOrientation();
	if (orientation == EnhancedSide::left) {
		// A is top end
		xGrowDirEndA = 0;
		yGrowDirEndA = -1;
		xGrowDirEndB = 0;
		yGrowDirEndB = +1;
		xShiftDir = -1;
		yShiftDir = 0;
		facingSideOrientation = EnhancedSide::right;
	}
	else if (orientation == EnhancedSide::top) {
		// A is left end
		xGrowDirEndA = -1;
		yGrowDirEndA = 0;
		xGrowDirEndB = +1;
		yGrowDirEndB = 0;
		xShiftDir = 0;
		yShiftDir = -1;
		facingSideOrientation = EnhancedSide::bottom;
	}
	else if (orientation == EnhancedSide::right) {
		// A is top end
		xGrowDirEndA = 0;
		yGrowDirEndA = -1;
		xGrowDirEndB = 0;
		yGrowDirEndB = +1;
		xShiftDir = +1;
		yShiftDir = 0;
		facingSideOrientation = EnhancedSide::left;
	}
	else if (orientation == EnhancedSide::bottom) {
		// A is left end
		xGrowDirEndA = -1;
		yGrowDirEndA = 0;
		xGrowDirEndB = +1;
		yGrowDirEndB = 0;
		xShiftDir = 0;
		yShiftDir = +1;
		facingSideOrientation = EnhancedSide::top;
	}

	// directions
	sideGrowDirs.first.first = xGrowDirEndA;
	sideGrowDirs.first.second = yGrowDirEndA;
	sideGrowDirs.second.first = xGrowDirEndB;
	sideGrowDirs.second.second = yGrowDirEndB;

	// search for optimal build pattern which has
	// complete coverage and no gap, if that can't
	// be achieved, then return best fit

	// start with base tile
	endTiles.first = endTiles.second = baseTile;

	// start growing, if coverage doesn't increase
	// overall chances are growth needs to be in opposite
	// direction of the chokepoint, so reverse
	// Otherwise stop growing when coverage stops
	// increasing or is complete (return length == 0) 

	// get initial coverage first (baseline)
	// 	minLengthNotCovered = currentLengthNotCovered;
	minLengthNotCovered = sideCoverage(ecpSide, endTiles);
	poorCoverage = false;
	reverseGrowDirection = false;

	// grow
	currentLengthNotCovered = growEndsByCoverage(ecpSide, endTiles, baseTile, minLengthNotCovered, sideGrowDirs);

	// set baselines
	minLengthNotCovered = currentLengthNotCovered;
	// check gap based on side ends
	sideEndGaps = sideEndsMinGap(ecpSide->isHorizontal(), endTiles);
	minTileGapSideA = sideEndGaps.first;
	minTileGapSideB = sideEndGaps.second;
	minTileFacingGapSideA = facingSideMinGap(facingSideOrientation, endTiles.first);
	minTileFacingGapSideB = facingSideMinGap(facingSideOrientation, endTiles.second);

	// poor coverage if nothing is covered, ie the length
	if (currentLengthNotCovered == ecpSide->getLengthPixels()) {
		poorCoverage = true;
	}
	else if (currentLengthNotCovered >= minLengthNotCovered) {
		tryShifting = true;
	}
	else {

		// if poor gap which is at least a build area's worth (still room to grow)
		// gap is now mearsured in walkable tiles, where a build tile is 
		// 4 walkable tiles wide
		poorSideEndGap = false;
		if (ecpSide->isHorizontal()) {
			if (minTileGapSideA >= buildWidth * 4 || minTileGapSideB >= buildWidth * 4) {
				poorSideEndGap = true;
			}
		}
		else {
			if (minTileGapSideA >= buildHeight * 4 || minTileGapSideB >= buildHeight * 4) {
				poorSideEndGap = true;
			}
		}

		// check for bump
		if (!poorSideEndGap) {
			growEndsByBumping(ecpSide->isHorizontal(), endTiles, sideGrowDirs, minTileGapSideA, minTileGapSideB);
		}

	} // end first grow attempt

	// IF POOR COVERAGE DO A MAX GROW JUST IN CASE
	// baseTile doesn't sit near the chokepoint side
	// if that fails then do reverse grow
	if (poorCoverage) {
		numBuildTiles = growEndsByLength(endTiles, sideGrowDirs);
		if (numBuildTiles > 4) {
			maxGrow = true;
			endTiles.first = endTiles.second = baseTile;
		}
		else {
			poorCoverage = false;
			// check for bumping case
			growEndsByBumping(ecpSide->isHorizontal(), endTiles, sideGrowDirs, minTileGapSideA, minTileGapSideB);
		}

		// THERE IS THE CASE THAT THERE IS ONLY 4 BUILD TILES, BUT IT'S THE WRONG DIR
		// still need to check the facing side gap, if too much try reverse grow
		currentTileFacingGapSideA = facingSideMinGap(facingSideOrientation, endTiles.first);
		currentTileFacingGapSideB = facingSideMinGap(facingSideOrientation, endTiles.second);
		if ((currentTileFacingGapSideA / 4) > 11 || (currentTileFacingGapSideB / 4) > 11) {
			maxGrow = true;
		}
	}




	// regrow in other direction
	// ie if horizontal, do vertical
	if (maxGrow) {

		reverseGrowDirection = true;
		// not applicable anymore
		poorCoverage = false;

		// leaving out shift dir, since that is unknown now
		if (ecpSide->isHorizontal()) {
			xGrowDirEndA = 0;
			yGrowDirEndA = -1;
			xGrowDirEndB = 0;
			yGrowDirEndB = +1;

			// vertical growth
			// compare ecpSide and baseTile x values
			// to determine left or right for facing
			// side orientation
			if (ecpSide->getEndPoints().second.x() < baseTile.x() * 32) {
				// it's left, so face left
				facingSideOrientation = EnhancedSide::left;	
				xShiftDir = 1;
				yShiftDir = 0;
			}
			else {
				facingSideOrientation = EnhancedSide::right;
				xShiftDir = -1;
				yShiftDir = 0;
			}
		}
		else {
			xGrowDirEndA = -1;
			yGrowDirEndA = 0;
			xGrowDirEndB = +1;
			yGrowDirEndB = 0;

			// horizontal growth
			// compare ecpSide and baseTile y values
			// to determine top or bottom for facing
			// side orientation
			if (ecpSide->getEndPoints().second.y() < baseTile.y() * 32) {
				// it's above, so face top
				facingSideOrientation = EnhancedSide::top;	
				xShiftDir = 0;
				yShiftDir = 1;
			}
			else {
				facingSideOrientation = EnhancedSide::bottom;
				xShiftDir = 0;
				yShiftDir = -1;
			}
		}

		sideGrowDirs.first.first = xGrowDirEndA;
		sideGrowDirs.first.second = yGrowDirEndA;
		sideGrowDirs.second.first = xGrowDirEndB;
		sideGrowDirs.second.second = yGrowDirEndB;

		// start with base tile
		endTiles.first = endTiles.second = baseTile;

		// regrow
		poorFacingSideGap = growEndsByGap(facingSideOrientation, endTiles, baseTile, 
			sideGrowDirs, currentTileFacingGapSideA, currentTileFacingGapSideB);

		// need to compare minTileFacingGapSideA, minTileFacingGapSideB
		// if any improvement

		// set baselines
		if (currentTileFacingGapSideA + currentTileFacingGapSideB <
			minTileFacingGapSideA, minTileFacingGapSideB) {

				maxGrow = false;
				minTileFacingGapSideA = currentTileFacingGapSideA;
				minTileFacingGapSideB = currentTileFacingGapSideB;
				// use opposite of ecpSide
				sideEndGaps = sideEndsMinGap(!ecpSide->isHorizontal(), endTiles);
				minTileGapSideA = sideEndGaps.first;
				minTileGapSideB = sideEndGaps.second;

				if (poorFacingSideGap || minTileGapSideA / 4 > 0 || minTileGapSideB / 4 > 0) {
					numBuildTiles = growEndsByLength(endTiles, sideGrowDirs);
					if (numBuildTiles > 4) {
						maxGrow = true;
						endTiles.first = endTiles.second = baseTile;
					}
					else {
						// use opposite of ecpSide
						growEndsByBumping(!ecpSide->isHorizontal(), endTiles, sideGrowDirs, minTileGapSideA, minTileGapSideB);
					}
					// if still issues, then try shifting, just can't do coverage

				}
		}
	} // end reverse regrow


	// if good coverage and poor side gap
	// try to grow more but look at the facing gap
	if (poorSideEndGap && !reverseGrowDirection) {
		poorFacingSideGap = growEndsByGap(facingSideOrientation, endTiles, baseTile, 
			sideGrowDirs, currentTileFacingGapSideA, currentTileFacingGapSideB);

		// if this is ok the reset poorSideEndGap to TRUE!
		if (!poorFacingSideGap) {
			poorSideEndGap = false;
			minTileFacingGapSideA = currentTileFacingGapSideA;
			minTileFacingGapSideB = currentTileFacingGapSideB;

			// check for bumping
			growEndsByBumping(ecpSide->isHorizontal(), endTiles, sideGrowDirs, minTileGapSideA, minTileGapSideB);
		}
	}

	// try MAX GROW and then reset poorSideEndGap and poorFacingSideGap 
	// otherwise tryShifting
	if (poorFacingSideGap) {
		numBuildTiles = growEndsByLength(endTiles, sideGrowDirs);
		if (numBuildTiles > 4) {
			maxGrow = true;
			endTiles.first = endTiles.second = baseTile;
		}
		else {
			poorFacingSideGap = false;
			poorSideEndGap = false;
			// check for bumping
			growEndsByBumping(ecpSide->isHorizontal(), endTiles, sideGrowDirs, minTileGapSideA, minTileGapSideB);
		}
	}



	// try to shift and then grow again 
	//(except if reverse - another special case)
	// this will end up being a reverse shift
	// can't use coverage on grow since it will always be wrong
	// as growth is parallel to chokepoint side
	if (tryShifting) {

		// start with base tile
		currentTile = baseTile;

		for (int m = 0; m < 2 && tryShifting; ++m) {

			currentTile.x() += xShiftDir;
			currentTile.y() += yShiftDir;

			// save previous, in case it get's worse
			prevEndTiles = endTiles;
			prevTileFacingGapSideA = minTileFacingGapSideA;
			prevTileFacingGapSideB = minTileFacingGapSideB;
			prevTileGapSideA = minTileGapSideA;
			prevTileGapSideB = minTileGapSideB;

			endTiles.first = endTiles.second = currentTile;
			// should be the same with change of baseTile
			minLengthNotCovered = sideCoverage(ecpSide, endTiles);
			poorCoverage = false;
			poorSideEndGap = false;
			poorFacingSideGap = false;
			tryShifting = false;

			// check buildability of current tile first
			if (BWAPI::Broodwar->canBuildHere(NULL, currentTile, BWAPI::UnitTypes::Terran_Supply_Depot, false)) {

				if (!reverseGrowDirection) {
					// use currentTile instead of baseTile for growEnds()
					currentLengthNotCovered = growEndsByCoverage(ecpSide, endTiles, currentTile, minLengthNotCovered, sideGrowDirs);

					if (currentLengthNotCovered >= minLengthNotCovered) {
						tryShifting = true;
					}
					else {
						minLengthNotCovered = currentLengthNotCovered;

						// check gap based on side ends
						sideEndGaps = sideEndsMinGap(ecpSide->isHorizontal(), endTiles);
						minTileGapSideA = sideEndGaps.first;
						minTileGapSideB = sideEndGaps.second;

						// if poor gap which is at least a build area's worth (still room to grow)
						if (ecpSide->isHorizontal()) {
							if (minTileGapSideA >= buildWidth * 4 || minTileGapSideB >= buildWidth * 4) {
								poorSideEndGap = true;
							}
						}
						else {
							if (minTileGapSideA >= buildHeight * 4 || minTileGapSideB >= buildHeight * 4) {
								poorSideEndGap = true;
							}
						}

					} // end good coverage
				}
			}

			// if good coverage and poor side gap after shift check
			// try to grow more but look at the facing gap
			if (poorSideEndGap || reverseGrowDirection) {
				// use currentTile instead of baseTile for growEnds()
				poorFacingSideGap = growEndsByGap(facingSideOrientation, endTiles, currentTile, sideGrowDirs, 
					currentTileFacingGapSideA, currentTileFacingGapSideB);

				// if this is ok the reset poorSideEndGap to TRUE!
				if (!poorFacingSideGap) {
					poorSideEndGap = false;
					minTileFacingGapSideA = currentTileFacingGapSideA;
					minTileFacingGapSideB = currentTileFacingGapSideB;
				}
			}

			// try MAX GROW and then reset poorSideEndGap and poorFacingSideGap 
			// otherwise keep shifting
			if (poorFacingSideGap) {
				// endTiles should be in accordance with the shifted baseTile ?
				numBuildTiles = growEndsByLength(endTiles, sideGrowDirs);
				if (numBuildTiles > 4) {
					maxGrow = true;
					endTiles.first = endTiles.second = baseTile;
				}
				else {
					poorFacingSideGap = false;
					poorSideEndGap = false;
				}
			}

			// check for bumping
			if (reverseGrowDirection) {
				growEndsByBumping(!ecpSide->isHorizontal(), endTiles, sideGrowDirs, minTileGapSideA, minTileGapSideB);
			}
			else {
				growEndsByBumping(ecpSide->isHorizontal(), endTiles, sideGrowDirs, minTileGapSideA, minTileGapSideB);
			}
			// check for side end or facing side gaps over a tile length,
			// if an end has neither closed off, tryShifting again
			// but reset if it gets worse (endTiles is overwritten)
			if (reverseGrowDirection) {
				sideEndGaps = sideEndsMinGap(!ecpSide->isHorizontal(), endTiles);
			}
			else {
				sideEndGaps = sideEndsMinGap(ecpSide->isHorizontal(), endTiles);
			}
			currentTileGapSideA = sideEndGaps.first;
			currentTileGapSideB = sideEndGaps.second;
			currentTileFacingGapSideA = facingSideMinGap(facingSideOrientation, endTiles.first);
			currentTileFacingGapSideB = facingSideMinGap(facingSideOrientation, endTiles.second);


		} // end for loop
	} // end shift check


	// -- get this section working first

	// if still not optimal, try reverse shifting ( after reverse growing)

	// if still not optimal, try staggered pattern

	// if everything is good add to buildTiles
	if (!poorCoverage && !poorSideEndGap && !poorFacingSideGap && !maxGrow) {
		currentTile = endTiles.first;
		count = 0;
		buildTiles.clear();
		while (currentTile != endTiles.second) {
			buildTiles.push_back(currentTile);
			currentTile.x() += xGrowDirEndB * buildWidth;
			currentTile.y() += yGrowDirEndB * buildHeight;
			count++;
			// check for overrun
			if (count > 10) {
				break;
			}
		}
		buildTiles.push_back(endTiles.second);
	}


	// did we find anything ?
	if ((int)buildTiles.size() == 0) {
		buildTiles.push_back(baseTile);
	}

	return buildTiles;
}

void TacticalBuildingPlacer::analyze(void)
{


}

void TacticalBuildingPlacer::draw(void)
{
	if( DebugFlags::instance().getFlag(none) )
		return;

	if( !DebugFlags::instance().getFlag(reserve_map) )
		return;

	BWTA::Region * region;
	vector<EnhancedChokepoint> * chokepoints;
	EnhancedUI * eui = new EnhancedUI();
	const BWAPI::UnitType & depot = BWAPI::UnitTypes::Terran_Supply_Depot;

	for (int m = 0; m < (int) regionsToChokepoints.size(); ++m) {

		region = regionsToChokepoints[m].first;
		chokepoints = &regionsToChokepoints[m].second;

		/* draw for enhanced chokepoints */
		for (int x = 0; x < (int) chokepoints->size(); ++x) {
			/* draw tiles for the sides and center of chokepoint line */
			chokepoints[0][x].drawTilePositions();
			/* leave out for now, sides can be hard to see with this on */
			//ecPoints[x].drawBoundingBox();
			chokepoints[0][x].drawBoundingBoxSidesForRegion(region, BWAPI::Colors::Red);
			chokepoints[0][x].drawBuildableTilesForRegion(region, BWAPI::Colors::White);
			//chokepoints[0][x].drawBuildableSupplyDepotForRegion(region, BWAPI::Colors::Cyan);
		}
	}

	// draw for chokepointBuildPatternSearch() return
	for (int m = 0; m < (int)chokepointBuildLocations.size(); ++m) {
		for (int n = 0; n < (int)chokepointBuildLocations[m].size(); ++n) {
			eui->drawBoxAtTilePositionToSize(chokepointBuildLocations[m][n], depot.tileWidth(), 
				depot.tileHeight(), BWAPI::Colors::Cyan);
		}
	}

	// clean up
	delete(eui);
}

// draw reserved tiles
void TacticalBuildingPlacer::update(void)
{
	if( DebugFlags::instance().getFlag(none) )
		return;

	if( !DebugFlags::instance().getFlag(reserve_map) )
		return;

	// draw reserved tiles
	for (int x = 0; x < BWAPI::Broodwar->mapWidth(); x++)
		for (int y = 0; y < BWAPI::Broodwar->mapHeight(); y++)
			if (BWSAL::ReservedMap::getInstance()->isReserved(x, y))
				BWAPI::Broodwar->drawBoxMap(x*32, y*32, x*32+31, y*32+31, BWAPI::Colors::Red);
}

// this is also the entry point for special building placement
BWAPI::TilePosition TacticalBuildingPlacer::reserveBuildLocation(BWAPI::UnitType unitType, BWAPI::TilePosition seedLocation, BWAPI::Unit *builder)
{
	BWAPI::TilePosition loc;

	// Find a default location first
	loc = buildingPlacer.findBuildLocation(BWSAL::ReservedMap::getInstance(), unitType, seedLocation, builder);

	// special build case for refineries
	if (unitType == BWAPI::UnitTypes::Terran_Refinery) {
		// hardcode for now, for start location
		BWTA::BaseLocation * baseLoc;
		baseLoc = BWTA::getStartLocation(BWAPI::Broodwar->self());
		// geysers per base location
		set<BWAPI::Unit*> geysers = baseLoc->getGeysers();
		// should only be one ?
		for(std::set<BWAPI::Unit*>::const_iterator j=geysers.begin(); j != geysers.end(); ++j) {
			loc =(*j)->getTilePosition();
			break;
		}
	}
	// Expansions
	else if (unitType == BWAPI::UnitTypes::Terran_Command_Center)
	{
		double minDist = 9999999;
		set<BWTA::BaseLocation*> expansions = BWTA::getBaseLocations();
		for (set<BWTA::BaseLocation*>::iterator it = expansions.begin(); it != expansions.end(); it++)
		{
			bool isCC = false;
			// Don't rebuild at a base location
			UnitSet units = Broodwar->getUnitsInRadius((*it)->getPosition(), 200);
			for (UnitSetIter u = units.begin(); u != units.end(); u++)
			{
				if ((*u)->getType().getID() == UnitTypes::Terran_Command_Center)
				{
					isCC = true;
					break;
				}
			}
			if (isCC)
				continue;

			// Otherwise, find the closest
			double dist = (*it)->getGroundDistance(BWTA::getStartLocation(BWAPI::Broodwar->self()));
			if (dist < minDist && 
				dist > 200) // Don't rebuild at a location
			{
				minDist = dist;
				loc = (*it)->getTilePosition();
			}
		}
	}
	// Bunkers
	else if (unitType == BWAPI::UnitTypes::Terran_Bunker)
	{
		pair<BWTA::Region *, vector<EnhancedChokepoint>> regionChokepoints;
		BWTA::BaseLocation * baseLoc;
		BWTA::Region * baseRegion;

		baseLoc = BWTA::getStartLocation(BWAPI::Broodwar->self());
		baseRegion = baseLoc->getRegion();

		// check for base region chokepoints, currently this is the only one
		if (regionsToChokepoints[0].first == baseRegion) {
			regionChokepoints = regionsToChokepoints[0];
			for (int m = 0; m < (int) regionChokepoints.second.size(); ++m) {
				// EnhancedChokepoint build tiles are setup for supply depots, so for now
				// a bunker will be able to fit into that space
				BWAPI::TilePosition buildPosition = regionChokepoints.second[m].getBuildTile(regionChokepoints.first);
				// see if tile position has already been reserved
				if (!BWSAL::ReservedMap::getInstance()->isReserved(buildPosition)) {
					loc = buildPosition;
					break;
				}
			}
		}
	}

	// reserve location 
	BWSAL::ReservedMap::getInstance()->reserveTiles(loc, unitType, unitType.tileWidth(), unitType.tileHeight());

	return loc;
}
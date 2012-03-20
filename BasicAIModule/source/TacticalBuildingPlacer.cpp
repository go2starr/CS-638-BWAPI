#pragma once
/*
* TacticalBuildingPlacer.cpp
*/

#include <TacticalBuildingPlacer.h>
#include <BWSAL/BFSBuildingPlacer.h>
#include <BWAPI.h>

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

// search for optimal build pattern to wall off chokepoint in a region
vector<BWAPI::TilePosition> TacticalBuildingPlacer::chokepointBuildPatternSearch(
	EnhancedChokepoint ecpoint, BWTA::Region * region) 
{
	vector<BWAPI::TilePosition> buildTiles;
	//vector<BWAPI::TilePosition> currentBuildTiles;
	// side A and B
	pair<BWAPI::TilePosition, BWAPI::TilePosition> endTiles;
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
	int minLengthNotCovered, currentLengthNotCovered;
	// tiles
	int minTileGapSideA, minTileGapSideB;
	int currentTileGapSideA, currentTileGapSideB;
	int ecpNumSides;
	EnhancedSide::Orientation orientation;
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
	// quickly tell if wall is far away on growth
	bool hitMax;
	// make sure 2nd direction isn't really bad either
	bool hitMaxReverse;
	// count growth
	int buildCountSideA, buildCountSideB;
	// general purpose
	int count;

	assert(region);


	baseTile = ecpoint.getBuildTile(region);
	ecpSides = ecpoint.getBoundingBoxSidesForRegion(region);
	buildWidth = depot.tileWidth();
	buildHeight = depot.tileHeight();
	ecpNumSides = (int)ecpSides.size();

	// only handles 2 sides
	assert(ecpNumSides && ecpNumSides <= 2);

	// if sides, search on longest otherwise
	// if the 2 sides are equal in length
	// you'll almost always have to search in both directions
	// one way will have a lot of space and the other way a little
	// -- maybe do gap analysis to pick either side?
	// nope, as it turns out, one tile of the side can have gap 0
	// and another of gap 40 and checkGap() is set up for the min
	// TODO: set up gap check for max
	// otherwise hitMax check is in place
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
				// gap analysis for 4 sides of the build area from baseTile
				// if left (A) and right have min, then pick horizontal side
				// if top (A) and bottom have min, then pick vertical side
				// left side
				sideEndPoints.first.x() = baseTile.x() * 32;
				sideEndPoints.first.y() = baseTile.y() * 32;
				sideEndPoints.second.x() = sideEndPoints.first.x();
				sideEndPoints.second.y() = ((baseTile.y() + buildHeight) * 32) - 1;
				EnhancedSide left(sideEndPoints, EnhancedSide::left);
				minTileGapSideA = left.checkGap();
				// right side
				sideEndPoints.first.x() = ((baseTile.x() + buildWidth) * 32) - 1;
				sideEndPoints.first.y() = baseTile.y() * 32;
				sideEndPoints.second.x() = sideEndPoints.first.x();
				sideEndPoints.second.y() = ((baseTile.y() + buildHeight) * 32) - 1;
				EnhancedSide right(sideEndPoints, EnhancedSide::right);
				minTileGapSideB = right.checkGap();
				// top side
				sideEndPoints.first.x() = baseTile.x() * 32;
				sideEndPoints.first.y() = baseTile.y() * 32;
				sideEndPoints.second.x() = ((baseTile.x() + buildWidth) * 32) - 1;
				sideEndPoints.second.y() = sideEndPoints.first.y();
				EnhancedSide top(sideEndPoints, EnhancedSide::top);
				currentTileGapSideA = top.checkGap();
				// bottom side
				sideEndPoints.first.x() = baseTile.x() * 32;
				sideEndPoints.first.y() = ((baseTile.y() + buildHeight) * 32) - 1;
				sideEndPoints.second.x() = ((baseTile.x() + buildWidth) * 32) - 1;
				sideEndPoints.second.y() = sideEndPoints.first.y();
				EnhancedSide bottom(sideEndPoints, EnhancedSide::bottom);
				currentTileGapSideB = bottom.checkGap();
				// pick horizontal side
				if (minTileGapSideA + minTileGapSideB > currentTileGapSideA + currentTileGapSideB) {
					if (ecpSides[0].isHorizontal()) {
						ecpSide = &(ecpSides[0]);
					}
					else {
						ecpSide = &(ecpSides[1]);
					}
				}
				else {
					if (!ecpSides[0].isHorizontal()) {
						ecpSide = &(ecpSides[0]);
					}
					else {
						ecpSide = &(ecpSides[1]);
					}
				}
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
	}
	else if (orientation == EnhancedSide::top) {
		// A is left end
		xGrowDirEndA = -1;
		yGrowDirEndA = 0;
		xGrowDirEndB = +1;
		yGrowDirEndB = 0;
		xShiftDir = 0;
		yShiftDir = -1;
	}
	else if (orientation == EnhancedSide::right) {
		// A is top end
		xGrowDirEndA = 0;
		yGrowDirEndA = -1;
		xGrowDirEndB = 0;
		yGrowDirEndB = +1;
		xShiftDir = +1;
		yShiftDir = 0;
	}
	else if (orientation == EnhancedSide::bottom) {
		// A is left end
		xGrowDirEndA = -1;
		yGrowDirEndA = 0;
		xGrowDirEndB = +1;
		yGrowDirEndB = 0;
		xShiftDir = 0;
		yShiftDir = +1;
	}

	// search for optimal build pattern which has
	// complete coverage and no gap, if that can't
	// be achieved, then return best fit

	// start with base tile
	endTiles.first = endTiles.second = baseTile;
	// see if either side builds out too far
	hitMax = false;

	// try to grow on either end
	// set growth limit of 4 on either end
	// then check for hit max

	// grow end A
	currentTile = baseTile;
	canBuild = true;
	buildCountSideA = 0;

	while (canBuild && buildCountSideA < 5) {
		currentTile.x() += xGrowDirEndA * buildWidth;
		currentTile.y() += yGrowDirEndA * buildHeight;

		if (BWAPI::Broodwar->canBuildHere(NULL, currentTile, BWAPI::UnitTypes::Terran_Supply_Depot, false)) {
			endTiles.first = currentTile;
			buildCountSideA++;
		}
		else {
			canBuild = false;
			break;
		}
	}

	// grow end B
	currentTile = baseTile;
	canBuild = true;
	buildCountSideB = 0;

	while (canBuild && buildCountSideB < 5) {
		currentTile.x() += xGrowDirEndB * buildWidth;
		currentTile.y() += yGrowDirEndB * buildHeight;

		if (BWAPI::Broodwar->canBuildHere(NULL, currentTile, BWAPI::UnitTypes::Terran_Supply_Depot, false)) {
			endTiles.second = currentTile;
			buildCountSideB++;
		}
		else {
			canBuild = false;
			break;
		}
	}

	// good indication need to search in other direction
	// build counts don't usually count the baseTile,
	// since they are searching out from it, so >3 is
	// really >4
	if (buildCountSideA + buildCountSideB > 3) {
		hitMax = true;
	}

	// regrow in other direction if hitMax before analysis
	// ie if horizontal, do vertical
	if (hitMax) {
		// leaving out shift dir, since that is unknown now
		if (ecpSide->isHorizontal()) {
			xGrowDirEndA = 0;
			yGrowDirEndA = -1;
			xGrowDirEndB = 0;
			yGrowDirEndB = +1;
		}
		else {
			xGrowDirEndA = -1;
			yGrowDirEndA = 0;
			xGrowDirEndB = +1;
			yGrowDirEndB = 0;
		}

		// start with base tile
		endTiles.first = endTiles.second = baseTile;
		hitMaxReverse = false;

		// now regrow end A
		currentTile = baseTile;
		canBuild = true;
		buildCountSideA = 0;
		while (canBuild && buildCountSideA < 5) {
			currentTile.x() += xGrowDirEndA * buildWidth;
			currentTile.y() += yGrowDirEndA * buildHeight;
			if (BWAPI::Broodwar->canBuildHere(NULL, currentTile, BWAPI::UnitTypes::Terran_Supply_Depot, false)) {
				endTiles.first = currentTile;
				buildCountSideA++;
			}
			else {
				canBuild = false;
				break;
			}
		}
		// regrow end B
		currentTile = baseTile;
		canBuild = true;
		buildCountSideB = 0;
		while (canBuild && buildCountSideB < 5) {
			currentTile.x() += xGrowDirEndB * buildWidth;
			currentTile.y() += yGrowDirEndB * buildHeight;
			if (BWAPI::Broodwar->canBuildHere(NULL, currentTile, BWAPI::UnitTypes::Terran_Supply_Depot, false)) {
				endTiles.second = currentTile;
				buildCountSideB++;
			}
			else {
				canBuild = false;
				break;
			}
		}
		if (buildCountSideA + buildCountSideB > 3) {
			hitMaxReverse = true;
		}
	} // end hitMax regrow


	// check coverage and gap based on side ends
	// Note: currently gap tests are only done for 
	// the side ends (the short ends), not the long ends
	// so this won't cover the case where there is still room
	// on either side of the side ends, but there isn't a gap on 
	// the long edge, so the chokepoint is still walled off, 
	// however this case will still come out as complete coverage
	// so coverage is more for the special cases
	if ((ecpSide->isHorizontal() && !hitMax) || (!ecpSide->isHorizontal() && hitMax)) {
		//  -------------
		//A |   |   |   | B
		// left to right, top or bottom side doesn't matter
		// get from endTiles, far left
		sideEndPoints.first.x() = endTiles.first.x() * 32;
		sideEndPoints.first.y() = endTiles.first.y() * 32;
		// get from endTiles, far right
		sideEndPoints.second.x() = ((endTiles.second.x() + buildWidth) * 32) - 1;
		sideEndPoints.second.y() = sideEndPoints.first.y(); // same
		// check coverage
		EnhancedSide coveringSide(sideEndPoints, EnhancedSide::top);
		currentSidesNotCovered.clear();
		currentLengthNotCovered = ecpSide->checkCoverage(coveringSide, &currentSidesNotCovered);

		// check side A gap
		// first end point is still good from coverage
		sideEndPoints.second.x() = sideEndPoints.first.x(); // same
		sideEndPoints.second.y() = sideEndPoints.first.y() + (buildHeight * 32) - 1;
		EnhancedSide buildSideA(sideEndPoints, EnhancedSide::left);
		currentTileGapSideA = buildSideA.checkGap();


		// check side B gap
		sideEndPoints.first.x() = ((endTiles.second.x() + buildWidth) * 32) - 1;
		// sideEndPoints.first.y() same
		sideEndPoints.second.x() = sideEndPoints.first.x(); // same
		// sideEndPoints.second.y() same
		EnhancedSide buildSideB(sideEndPoints, EnhancedSide::right);
		currentTileGapSideB = buildSideB.checkGap();
	}
	else if ( (!hitMax) || (ecpSide->isHorizontal() && hitMax) ){

		// A
		// ---
		// |
		// ---
		// |
		// ---
		// |
		// ---
		// B
		// get left side and compare for coverage
		// endTile A (first) is top
		sideEndPoints.first.x() = endTiles.first.x() * 32;
		sideEndPoints.first.y() = endTiles.first.y() * 32;
		// get from endTiles, far bottom
		sideEndPoints.second.x() = sideEndPoints.first.x(); // same
		sideEndPoints.second.y() = ((endTiles.second + buildHeight) * 32) - 1;
		// check coverage
		EnhancedSide coveringSide(sideEndPoints, EnhancedSide::left);
		currentSidesNotCovered.clear();
		currentLengthNotCovered = ecpSide->checkCoverage(coveringSide, &currentSidesNotCovered);

		// check side A gap
		// first end point is still good from coverage
		sideEndPoints.second.x() = sideEndPoints.first.x() + (buildWidth * 32) - 1;
		sideEndPoints.second.y() = sideEndPoints.first.y(); // same
		EnhancedSide buildSideA(sideEndPoints, EnhancedSide::top);
		currentTileGapSideA = buildSideA.checkGap();

		// check side B gap
		// sideEndPoints.first.x() same
		sideEndPoints.first.y() = ((endTiles.second + buildHeight) * 32) - 1;
		// sideEndPoints.second.x() same
		sideEndPoints.second.y() = sideEndPoints.first.y(); // same
		EnhancedSide buildSideB(sideEndPoints, EnhancedSide::bottom);
		currentTileGapSideB = buildSideB.checkGap();
	}

	// save round -- need to do some error checking here
	currentTile = endTiles.first;
	count = 0;
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

	// analyze coverage and side gaps
	// if both are zero, done!
	// make sure size isn't over 4, otherwise this may not be a good solution
	if (currentLengthNotCovered < 32 && currentTileGapSideA + currentTileGapSideB == 0) {
		if (buildTiles.size() < 5) {
			return buildTiles;
		}
		else {
			buildTiles.clear();
			buildTiles.push_back(baseTile);
			return buildTiles;
		}
	}

	// if first round set mins to current
	minLengthNotCovered = currentLengthNotCovered;
	minTileGapSideA = currentTileGapSideA;
	minTileGapSideB = currentTileGapSideB;

	// check to see if bumping the set by 1 or 2 tiles 
	// will open up another build position and close the gap
	// if horizontal, look for gap total of 3
	// if vertical, look for gap total of 2 (1 on either side)
	if ((ecpSide->isHorizontal() && minTileGapSideA + minTileGapSideB == buildWidth) ||
		(!ecpSide->isHorizontal() && minTileGapSideA + minTileGapSideB == buildHeight)) {
			// shift left 1 or 2 tiles and try to grow the right side
			currentTile.x() = endTiles.first.x() - minTileGapSideA;
			if (BWAPI::Broodwar->canBuildHere(NULL, currentTile, BWAPI::UnitTypes::Terran_Supply_Depot, false)) {
				endTiles.first = currentTile;
				// keep going and grow end B
				canBuild = true;
				buildCountSideB = 0;

				while (canBuild && buildCountSideB < 4) {
					currentTile.x() += xGrowDirEndB * buildWidth;
					currentTile.y() += yGrowDirEndB * buildHeight;

					if (BWAPI::Broodwar->canBuildHere(NULL, currentTile, BWAPI::UnitTypes::Terran_Supply_Depot, false)) {
						endTiles.second = currentTile;
						buildCountSideB++;
					}
					else {
						canBuild = false;
						break;
					}
				}
			}
	}

	// check for gap improvement after bumping

	if (ecpSide->isHorizontal()) {
		// check side A gap
		sideEndPoints.first.x() = endTiles.first.x() * 32;
		sideEndPoints.first.y() = endTiles.first.y() * 32;
		sideEndPoints.second.x() = sideEndPoints.first.x(); // same
		sideEndPoints.second.y() = sideEndPoints.first.y() + (buildHeight * 32) - 1;
		EnhancedSide buildSideA(sideEndPoints, EnhancedSide::left);
		currentTileGapSideA = buildSideA.checkGap();

		// check side B gap
		sideEndPoints.first.x() = ((endTiles.second.x() + buildWidth) * 32) - 1;
		// sideEndPoints.first.y() same
		sideEndPoints.second.x() = sideEndPoints.first.x(); // same
		// sideEndPoints.second.y() same
		EnhancedSide buildSideB(sideEndPoints, EnhancedSide::right);
		currentTileGapSideB = buildSideB.checkGap();
	}
	else {
		// check side A gap
		sideEndPoints.first.x() = endTiles.first.x() * 32;
		sideEndPoints.first.y() = endTiles.first.y() * 32;
		sideEndPoints.second.x() = sideEndPoints.first.x() + (buildWidth * 32) - 1;
		sideEndPoints.second.y() = sideEndPoints.first.y(); // same
		EnhancedSide buildSideA(sideEndPoints, EnhancedSide::top);
		currentTileGapSideA = buildSideA.checkGap();

		// check side B gap
		// sideEndPoints.first.x() same
		sideEndPoints.first.y() = ((endTiles.second + buildHeight) * 32) - 1;
		// sideEndPoints.second.x() same
		sideEndPoints.second.y() = sideEndPoints.first.y(); // same
		EnhancedSide buildSideB(sideEndPoints, EnhancedSide::bottom);
		currentTileGapSideB = buildSideB.checkGap();
	}

	if (currentTileGapSideA + currentTileGapSideB < minTileGapSideA + minTileGapSideB) {

		// save new round
		buildTiles.clear();
		currentTile = endTiles.first;
		count = 0;
		while (currentTile != endTiles.second) {
			buildTiles.push_back(currentTile);
			currentTile.x() += xGrowDirEndB * buildWidth;
			currentTile.y() += yGrowDirEndB * buildHeight;
			count++;
			// check for overrun, these tiles will then be thrown out
			// in the size check 
			if (count > 10) {
				break;
			}
		}
		buildTiles.push_back(endTiles.second);



		// NEED TO DO COVERAGE ANALYSIS AND
		// SET LENGTH NOT COVERED AND MIN TILE GAPS
	}

	// -- get this section working first

	// try to shift and then grow again

	// compare coverage and gap

	// if still not optimal, try opposite direction

	// if still not optimal, try staggered pattern

	// ensure size isn't to big
	if (buildTiles.size() < 5) {
		return buildTiles;
	}
	else {
		buildTiles.clear();
		buildTiles.push_back(baseTile);
	}

	return buildTiles;
}

void TacticalBuildingPlacer::analyze(void)
{


}

void TacticalBuildingPlacer::draw(void)
{
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
	for (int x = 0; x < BWAPI::Broodwar->mapWidth(); x++)
		for (int y = 0; y < BWAPI::Broodwar->mapHeight(); y++)
			if (BWSAL::ReservedMap::getInstance()->isReserved(x, y))
				BWAPI::Broodwar->drawBoxMap(x*32, y*32, x*32+31, y*32+31, BWAPI::Colors::Red);
}

BWAPI::TilePosition TacticalBuildingPlacer::reserveBuildLocation(BWAPI::UnitType unitType, BWAPI::TilePosition seedLocation, BWAPI::Unit *builder)
{
	BWAPI::TilePosition loc;

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
		BWSAL::ReservedMap::getInstance()->reserveTiles(loc, unitType, unitType.tileWidth(), unitType.tileHeight());
		return loc;	
	}

	loc = buildingPlacer.findBuildLocation(BWSAL::ReservedMap::getInstance(), unitType, seedLocation, builder);
	BWSAL::ReservedMap::getInstance()->reserveTiles(loc, unitType, unitType.tileWidth(), unitType.tileHeight());

	return loc;
}
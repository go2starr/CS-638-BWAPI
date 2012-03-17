/*
* EnhancedChokepoint.cpp
*/
#include "EnhancedChokepoint.h"

#include <BWAPI.h>
#include <BWTA.h>

#include <assert.h>

using std::pair;
using std::vector;


EnhancedChokepoint::EnhancedChokepoint(BWTA::Chokepoint * chokepoint)
{
	pair<BWAPI::Position, BWAPI::Position> sides;
	/* corners */
	BWAPI::Position topLeft, topRight, bottomLeft, bottomRight;
	/* sides */
	pair<BWAPI::Position, BWAPI::Position> leftSide, topSide, rightSide, bottomSide;
	/* regions */
	pair<BWTA::Region *, BWTA::Region *> regions;

	assert(chokepoint);
	cp = chokepoint;

	regions = cp->getRegions();
	sides = cp->getSides();

	/* get tile positions for center & sides */
	tPosCenter = BWAPI::TilePosition(cp->getCenter());
	tPosSides.first = BWAPI::TilePosition(sides.first);
	tPosSides.second = BWAPI::TilePosition(sides.second);

	/* get bounding box in position coords by tile positions */

	if(tPosSides.first.x() <= tPosSides.second.x()) {
		xLeft = tPosSides.first.x() * 32;
		xRight = ((tPosSides.second.x() + 1) * 32) - 1;
	}
	else {
		xLeft = tPosSides.second.x() * 32;
		xRight = ((tPosSides.first.x() + 1) * 32) - 1;
	}

	if(tPosSides.first.y() <= tPosSides.second.y()) {
		yTop = tPosSides.first.y() * 32;
		yBottom = ((tPosSides.second.y() + 1) * 32) - 1;
	}
	else {
		yTop = tPosSides.second.y() * 32;
		yBottom = ((tPosSides.first.y() + 1) * 32) - 1;
	}

	/* set corner positions */
	topLeft = BWAPI::Position(xLeft, yTop);
	topRight = BWAPI::Position(xRight, yTop);
	bottomLeft = BWAPI::Position(xLeft, yBottom);
	bottomRight = BWAPI::Position(xRight, yBottom);

	/* set sides, first position must be less
	* than the second (x or y), for searching purposes,
	* ie, only search right or down
	*/
	leftSide.first = topLeft;
	leftSide.second = bottomLeft;
	rightSide.first = topRight;
	rightSide.second = bottomRight;
	topSide.first = topLeft;
	topSide.second = topRight;
	bottomSide.first = bottomLeft;
	bottomSide.second = bottomRight;

	/* search for sides in region A */
	regionASides.first = regions.first;
	regionABuildableTiles.first = regions.first;
	regionABuildTile.first = regions.first; 

	if (isSideInRegion(leftSide, regions.first)) {
		regionASides.second.push_back(EnhancedSide(leftSide, EnhancedSide::left));
	}
	if (isSideInRegion(topSide, regions.first)) {
		regionASides.second.push_back(EnhancedSide(topSide, EnhancedSide::top));
	}
	if (isSideInRegion(rightSide, regions.first)) {
		regionASides.second.push_back(EnhancedSide(rightSide, EnhancedSide::right));
	}
	if (isSideInRegion(bottomSide, regions.first)) {
		regionASides.second.push_back(EnhancedSide(bottomSide, EnhancedSide::bottom));
	}


	/* serach for sides in region B */
	regionBSides.first = regions.second;
	regionBBuildableTiles.first = regions.second;
	regionBBuildTile.first = regions.second; 

	if (isSideInRegion(leftSide, regions.second)) {
		regionBSides.second.push_back(EnhancedSide(leftSide, EnhancedSide::left));
	}
	if (isSideInRegion(topSide, regions.second)) {
		regionBSides.second.push_back(EnhancedSide(topSide, EnhancedSide::top));
	}
	if (isSideInRegion(rightSide, regions.second)) {
		regionBSides.second.push_back(EnhancedSide(rightSide, EnhancedSide::right));
	}
	if (isSideInRegion(bottomSide, regions.second)) {
		regionBSides.second.push_back(EnhancedSide(bottomSide, EnhancedSide::bottom));
	}

	// search buildable tiles for each region 
	radialSweepSearch(regionASides);
	radialSweepSearch(regionBSides);

}

EnhancedChokepoint::~EnhancedChokepoint()
{
}

/* wrapper Chokepoint functions */
const pair<BWTA::Region *, BWTA::Region *> & EnhancedChokepoint::getRegions() 
{
	return cp->getRegions();
}

const pair<BWAPI::Position, BWAPI::Position> & EnhancedChokepoint::getSides() 
{
	return cp->getSides();
}

BWAPI::Position EnhancedChokepoint::getCenter() 
{
	return cp->getCenter();
}

double EnhancedChokepoint::getWidth() 
{
	return cp->getWidth();
}



/* enhanced functions */

// http://alienryderflex.com/polygon/
//  Globals which should be set before calling this function:
//
//  int    polySides  =  how many corners the polygon has
//  float  polyX[]    =  horizontal coordinates of corners
//  float  polyY[]    =  vertical coordinates of corners
//  float  x, y       =  point to be tested
//
//  (Globals are used in this example for purposes of speed.  Change as
//  desired.)
//
//  The function will return YES if the point x,y is inside the polygon, or
//  NO if it is not.  If the point is exactly on the edge of the polygon,
//  then the function may return YES or NO.
//
//  Note that division by zero is avoided because the division is protected
//  by the "if" clause which surrounds it.
bool EnhancedChokepoint::isPositionInPolygon(BWAPI::Position searchPos, BWTA::Polygon poly){

	int numPositions = (int) poly.size();
	int polySides = numPositions;
	vector<float>polyX;
	vector<float>polyY;
	float x, y;
	int      i, j;
	bool  oddNodes = false;
	BWAPI::Position pos;

	polyX.reserve(numPositions);
	polyY.reserve(numPositions);

	/* get position values from polygon */
	for (int m = 0; m < numPositions; ++m) {
		pos = poly[m];
		polyX.push_back((float) pos.x());
		polyY.push_back((float) pos.y());
	}

	/* get position to search for */
	x = (float) searchPos.x();
	y = (float) searchPos.y();


	/* init j */
	j = polySides - 1;

	for (i=0; i<polySides; i++) {
		if ((polyY[i]< y && polyY[j]>=y
			||   polyY[j]< y && polyY[i]>=y)
			&&  (polyX[i]<=x || polyX[j]<=x)) {
				oddNodes^=(polyX[i]+(y-polyY[i])/(polyY[j]-polyY[i])*(polyX[j]-polyX[i])<x); }
		j=i; }

	return oddNodes; 
}

// used to find buildable tiles and a build tile for a supply depot
void EnhancedChokepoint::radialSweepSearch(pair<BWTA::Region * , vector<EnhancedSide>> regionSides)
{

	int numSides;
	//	# of rounds to creep out
	int radiusLevel;
	// directions -1, +1, or 0
	int xDir, yDir;
	// used to find new start tile from
	// previous stat tile
	int xStartDir, yStartDir;
	bool horizontalSide;
	// stores if horizontal, xDir, and yDir
	// mainly to reset to the initial direction
	// state, but also if start tile ends up being
	// on a different side
	// side 0 state:
	// directionOfSides[0][0] = is horizontal
	// directionOfSides[0][1] = xDir
	// directionOfSides[0][2] = yDir
	vector<int>sideDirection;
	vector<vector<int>> directionOfSides;
	bool foundBuildTile;
	bool foundStartTile;
	bool lastTileBuildable;
	EnhancedSide::Orientation orientation;
	// what we are searching for, includes all
	// buildable tiles found in the search
	vector<BWAPI::TilePosition> buildableTiles;
	BWAPI::TilePosition buildTile;
	// last known good buildable tile
	BWAPI::TilePosition previousStartTile;
	// first tile to increment from
	// getting the current tile
	BWAPI::TilePosition startTile;
	// first good buildable tile found
	BWAPI::TilePosition newStartTile;
	// first tile to check for buildability
	BWAPI::TilePosition currentTile;
	// used in case no new start tiles found
	BWAPI::TilePosition unbuildableStartTile;
	// keep track of side lengths to know
	// when to change sides and direction
	vector<int> currentSideLengths;
	// derived from found buildable tiles
	vector<int> newSideLengths;
	// instead of individual side lengths
	// each accumulates from last side
	// derived from currentSideLengths
	vector<int> whenToChangeSideLengths;
	// total # of tiles (for all sides)
	int currentTileTotalCount;
	int newTileTotalCount;
	// current side we are on
	int currentSide;
	// number of new build tiles for this side
	int newSideLength;
	EnhancedUI * eui = new EnhancedUI(); 
	BWTA::Polygon poly = regionSides.first->getPolygon(); 

	// old comment:
	// sides are added from left to bottom in constructor
	// check tiles diagonally from end tiles, and from corner tiles
	// next level or radius of search is based upon the side orientation
	// if there are 2 sides, there is a corner tile adjoinig them (shared)
	// search in a radial pattern sweeping clockwise
	// stop sweeping if tile is unbuildable or a supply depot build tile is found
	// correction:  there is a case where the chokepoint is on a ramp, in which
	// case the entire first area may not be buildable, this makes things a bit
	// more complicated in knowing what is the set of tiles that should be considered
	// each round, if none are found to be buildable, then your set is 0, so if
	// none are found to be buildable, then you should just expand the set normally
	// as if they were all found to be buildable in hopes of the next round finding
	// at least one buildable tile (use unbuildableTilesForRound)

	numSides = (int) regionSides.second.size();
	//numCorners = numSides - 1;

	assert(numSides);

	// left start tile, -x, +y
	// left next tile x=0, -y (move up)
	// top start tile -x, -y
	// top next tile +x, y=0 (move right)
	// right start tile +x, -y
	// right next tile x=0, +y (move down)
	// bottom start tile +x, +y
	// bottom next tile -x, y=0 (move left)

	// default is unset
	xDir = yDir = xStartDir = yStartDir = 0;
	newTileTotalCount = 0;

	// sort through the sides
	for (int m = 0; m < numSides; ++m) {
		vector<BWAPI::TilePosition> sideTiles;
		sideTiles = regionSides.second[m].getTilePositions();

		// lengths of sides
		if (m == 0) {
			newSideLengths.push_back((int)sideTiles.size());
			newTileTotalCount += (int)sideTiles.size();
		}
		// next side length is reduced since it shares a tile
		// with the previous side
		else {
			newSideLengths.push_back((int)sideTiles.size() - 1);
			newTileTotalCount += (int)sideTiles.size() - 1;
		}
		orientation = regionSides.second[m].getOrientation();

		// only need the new start tile from the first side
		if (m > 0) {
			continue;
		}

		// establish the new start tile, and the directions
		if (orientation == EnhancedSide::left) {
			// left start tile, -x, +y
			// left next tile x=0, -y (move up)
			xStartDir = -1;
			yStartDir = +1;
			xDir = 0;
			yDir = -1;
			// reverse
			newStartTile = sideTiles.back();
			horizontalSide = false;
		}
		else if (orientation == EnhancedSide::top) {
			// top start tile -x, -y
			// top next tile +x, y=0 (move right)
			xStartDir = -1;
			yStartDir = -1;
			xDir = +1;
			yDir = 0;
			newStartTile = sideTiles.front();

			horizontalSide = true;

		}
		else if (orientation == EnhancedSide::right) {
			// right start tile +x, -y
			// right next tile x=0, +y (move down)
			xStartDir = +1;
			yStartDir = -1;
			xDir = 0;
			yDir = +1;
			newStartTile = sideTiles.front();
			horizontalSide = false;

		}
		else if (orientation == EnhancedSide::bottom) {
			// bottom start tile +x, +y
			// bottom next tile -x, y=0 (move left)
			xStartDir = +1;
			yStartDir = +1;
			xDir = -1;
			yDir = 0;
			newStartTile = sideTiles.back();
			//reverse = true;

			horizontalSide = true;
		}
	} // end search sides


	// may use this later to check initial side tiles for buildability
	// but not exactly this way, don't really care about the order...
	//	if (reverse) {

	//		for (int n = (int) sideTiles.size() - 1; n >= 0; --n) {
	//			if ( m > 0 && n == (int) sideTiles.size() - 1) {
	//				corners.push_back(sideTiles[n]);
	//				// and skip start of 2nd side (don't add twice since shared)
	//			}
	//			else {
	//				currentTilesToSearchFrom.push_back(sideTiles[n]);
	//			}
	//		}
	//	}
	//	else {

	//		for (int n = 0; n < (int) sideTiles.size(); ++n) {

	//			if (m > 0 && n == 0) {
	//				corners.push_back(sideTiles[n]);
	//				// and skip start of 2nd side (don't add twice since shared)
	//			}
	//			else {
	//				currentTilesToSearchFrom.push_back(sideTiles[n]);
	//			}
	//		}
	//	}
	//}

	// init vars
	foundBuildTile = false;
	radiusLevel = 0;
	newSideLength = 0;

	// set first sides direction
	sideDirection;
	sideDirection.clear();
	sideDirection.push_back((int)horizontalSide);
	sideDirection.push_back(xDir);
	sideDirection.push_back(yDir);
	directionOfSides.push_back(sideDirection);

	// now search until a build tile is found

	while (!foundBuildTile) {

		// reset before each round
		whenToChangeSideLengths.clear();
		currentSide = 0;
		// this also gets reset when the side changes
		newSideLength = 0;
		lastTileBuildable = false;
		// side direction (1 && for warning suppression)
		horizontalSide = (1 && directionOfSides[currentSide][0]);
		xDir = directionOfSides[currentSide][1];
		yDir = directionOfSides[currentSide][2];

		// not used currently, but maybe later
		if (radiusLevel > 0) {
			previousStartTile = startTile;
		}

		// if no buildable tiles are found in the round
		// continue as if all were found to be buildable
		if (radiusLevel > 0 && !foundStartTile) {
			startTile = unbuildableStartTile;
			currentTile = unbuildableStartTile;
			currentTileTotalCount += 2 * (int)currentSideLengths.size();
			int tileCount = 0;
			for (int n = 0; n < (int) currentSideLengths.size(); ++n) {
				// same # of sides as before, just longer, so no clear
				currentSideLengths[n] += 2;
				tileCount += currentSideLengths[n];
				whenToChangeSideLengths.push_back(tileCount - 1);
			}
		}
		else {
			startTile = newStartTile;
			// will be incremented on x and y
			currentTile = startTile;
			// what to grow out by
			currentTileTotalCount = newTileTotalCount + 2 * (int)newSideLengths.size();
			currentSideLengths.clear();
			int tileCount = 0; 
			for (int n = 0; n < (int) newSideLengths.size(); ++n) {
				currentSideLengths.push_back(newSideLengths[n] + 2);
				tileCount += newSideLengths[n] + 2;
				whenToChangeSideLengths.push_back(tileCount - 1);
			}

		}
		// reset round 
		// newSideLengths is set at the end of each round
		newSideLengths.clear();
		foundStartTile = false;
		newTileTotalCount = 0;


		// increment
		currentTile.x() += xStartDir;
		currentTile.y() += yStartDir;

		// start this round
		for (int m = 0; m < currentTileTotalCount; ++m) {

			// only used if no start tile is found in this round
			if (m == 0) {
				unbuildableStartTile = currentTile;
			}		

			if (BWAPI::Broodwar->isBuildable(currentTile, true) && 
				isPositionInPolygon(eui->getTilePositionCenter(currentTile), poly)) {
					buildableTiles.push_back(currentTile);
					newSideLength++;
					newTileTotalCount++;
					lastTileBuildable = true; // per round
					if (!foundStartTile) {
						newStartTile = currentTile;
						foundStartTile = true;

						// see if we are on a different side now, 
						// if so, need to change xStartDir and yStartDir
						// used saved directions, but also remove any
						// saved directions for sides that got eliminated
						if (currentSide > 0) {
							// shift all to the left by 1
							for (int n = 0; n < currentSide; ++n) {
								// from horizontal to vertical
								if (directionOfSides[n][0]) {
									xStartDir = xStartDir * -1;
								}
								else {
									yStartDir = yStartDir * -1;
								}
								// pop front
								directionOfSides[n] = directionOfSides[n+1];
							}
							// pop back, so no doubles
							directionOfSides.pop_back();
						}
					}

					if (BWAPI::Broodwar->canBuildHere(NULL, currentTile, BWAPI::UnitTypes::Terran_Supply_Depot, false)) {
						foundBuildTile = true; // yay!
						buildTile = currentTile;
						break;
					}
			}
			else if (lastTileBuildable) {
				// move on to next round since this tile
				// is not buildable and the last was
				//
				// Note: there can be the case where you
				// get a few buildable, then not buildable
				// then buildable again, but for now we want
				// to limit our search, unless this proves to 
				// be not so good, if then, the newSideLength
				// needs to be changed as it only counts for
				// buildable tiles
				break;
			}

			// check for change in direction for next side
			// if so push back size for newSideLengths
			if (m == (whenToChangeSideLengths[currentSide]) && 
				m != (currentTileTotalCount - 1)) { // skip on last tile

					newSideLengths.push_back(newSideLength);
					currentSide++;
					// reset for next side
					newSideLength = 0;

					// change direction based upon horizontality
					// how does this change per round?
					// what if the first side gets eliminated, that would
					// also change xStartDir and yStartDir for the start tile
					// find out if the start tile happens on the next side
					if (horizontalSide) {
						yDir = xDir;
						xDir = 0;
						// switch
						horizontalSide = false;
					}
					// vertical side
					else {
						// opposite
						xDir = yDir * -1;
						yDir = 0;
						// switch
						horizontalSide = true;
					}
					// save direction for side on first round
					// 1st side already saved, 2nd side gets added now and then so on
					// needs to happen after the next sides direction is derived
					if (radiusLevel == 0) {
						sideDirection;
						sideDirection.clear();
						sideDirection.push_back((int)horizontalSide);
						sideDirection.push_back(xDir);
						sideDirection.push_back(yDir);
						directionOfSides.push_back(sideDirection);
					}
			}

			// increment direction
			currentTile.x() += xDir;
			currentTile.y() += yDir;

		} // end round or radius level

		// now at the end of last side, either by
		// last tile or no more buildable tiles
		newSideLengths.push_back(newSideLength);

		// increment after each round
		radiusLevel++;

	} // end while()

	// save data

	if (regionSides.first == regionABuildableTiles.first) {
		regionABuildableTiles.second = buildableTiles;
		regionABuildTile.second = buildTile;
	}
	else {
		regionBBuildableTiles.second = buildableTiles;
		regionBBuildTile.second = buildTile;
	}

	// clean up
	delete(eui);

	return;

} // end func()


/*
* first position must be less then the second, ie always search down or right
* does not check the corner points, only the inside points
*/
bool EnhancedChokepoint::isSideInRegion(pair<BWAPI::Position, BWAPI::Position> endPoints, BWTA::Region * region)
{
	BWAPI::Position pos;
	BWTA::Polygon poly;

	// only want sides longer than a tile length
	// that way if the bounding box is a single row
	// of tiles, one long side should be in one region
	// the other long side should be in the other region,
	// we don't care about the 2 short, 1 tile length sides
	// these should be covered anyways when you get the tiles
	// for the long side, it's still the same tile
	//
	// fine / coarse grain size, 32 = 1 tile length
	// start + 32 = start of next tile 
	const int grainLengthForSearch = 32;

	assert(region);

	poly = region->getPolygon();

	if (endPoints.first.x() == endPoints.second.x()) {
		/* only search down */
		assert(endPoints.first.y() <= endPoints.second.y());
		/* search along y, x is constant */
		pos = BWAPI::Position(endPoints.first.x(), endPoints.first.y() + grainLengthForSearch);

		while (pos.y() < endPoints.second.y()) {
			// BWTA isInside() is broken, use ours
			//		if (poly.isInside(pos)) {
			if (isPositionInPolygon(pos, poly)) {
				return true;
			}
			pos.y() += grainLengthForSearch;
		}
	}
	else {
		/* only search right */
		assert(endPoints.first.x() <= endPoints.second.x());
		/* search along x, y is constant */
		pos = BWAPI::Position(endPoints.first.x() + grainLengthForSearch, endPoints.first.y());

		while (pos.x() < endPoints.second.x()) {
			// BWTA isInside() is broken, use ours
			//if (poly.isInside(pos)) {
			if (isPositionInPolygon(pos, poly)) {
				return true;
			}
			pos.x() += grainLengthForSearch;
		}
	}
	return false;
}
vector<EnhancedSide> & EnhancedChokepoint::getBoundingBoxSidesForRegion(BWTA::Region * region)
{

	assert(region);

	if (region == regionASides.first) {
		return regionASides.second;
	}

	return regionBSides.second;
}
void EnhancedChokepoint::drawBoundingBoxSidesForRegion(BWTA::Region * region, BWAPI::Color color)
{
	assert(region);

	if (region == regionASides.first) {
		for (int m = 0; m < (int) regionASides.second.size(); ++m) {
			regionASides.second[m].drawSide(color);
		}
	}
	else if (region == regionBSides.first) {
		for (int m = 0; m < (int) regionBSides.second.size(); ++m) {
			regionBSides.second[m].drawSide(color);
		}
	}
}
/* draws tile positions for the sides, and center */
void EnhancedChokepoint::drawTilePositions() 
{
	EnhancedUI * eui = new EnhancedUI();
	BWAPI::Color color = BWAPI::Colors::Purple;
	eui->drawTilePosition(this->tPosSides.first, color);
	eui->drawTilePosition(this->tPosSides.second, color);
	/* draw this last and in a different color */
	eui->drawTilePosition(this->tPosCenter, BWAPI::Colors::Yellow);
	delete eui;
}

void EnhancedChokepoint::drawBoundingBox(void)
{
	BWAPI::Broodwar->drawBoxMap(xLeft, yTop, xRight, yBottom, BWAPI::Colors::White, false);
}

void EnhancedChokepoint::drawBuildableTilesForRegion(BWTA::Region * region, BWAPI::Color color)
{
	assert(region);

	EnhancedUI * eui = new EnhancedUI();
	if (region == regionABuildableTiles.first) {
		for (int m = 0; m < (int)regionABuildableTiles.second.size(); ++m) {
			eui->drawTilePosition(regionABuildableTiles.second[m], color);
		}

	}
	else {
		for (int m = 0; m < (int)regionBBuildableTiles.second.size(); ++m) {
			eui->drawTilePosition(regionBBuildableTiles.second[m], color);
		}
	}
	delete eui;
}
void EnhancedChokepoint::drawBuildableSupplyDepotForRegion(BWTA::Region * region, 
														   BWAPI::Color color)
{
	assert(region);

	EnhancedUI * eui = new EnhancedUI();
	const BWAPI::UnitType & depot = BWAPI::UnitTypes::Terran_Supply_Depot;

	if (region == regionABuildTile.first) {

		//eui->drawTilePosition(regionABuildTile.second, color);
		eui->drawBoxAtTilePositionToSize(regionABuildTile.second, depot.tileWidth(), 
			depot.tileHeight(), color);
	}
	else {
		//eui->drawTilePosition(regionBBuildTile.second, color);
		eui->drawBoxAtTilePositionToSize(regionBBuildTile.second, depot.tileWidth(), 
			depot.tileHeight(), color);
	}
	delete eui;
}
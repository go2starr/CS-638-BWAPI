
/*
* EnhancedSide.cpp
*/
#include "EnhancedSide.h"

#include <BWAPI.h>
#include <BWTA.h>

#include <assert.h>

using std::pair;
using std::vector;

EnhancedSide::EnhancedSide(pair<BWAPI::Position, BWAPI::Position> endPoints, Orientation orientation)
{
	bool xMatch, yMatch;
	bool down, right;
	BWAPI::TilePosition * startTile, * endTile;
	BWAPI::TilePosition currTile;
	BWAPI::Position startWalkTile, currentWalkTile;

	down = right = false;

	xMatch = (endPoints.first.x() == endPoints.second.x());
	yMatch = (endPoints.first.y() == endPoints.second.y());

	/* ensure points share an x or y value */
	assert( xMatch || yMatch );

	/* set our endPoints */
	this->endPoints = endPoints;
	this->orientation = orientation;

	if (orientation == top || orientation == bottom) {
		horizontal = true;
	}
	else {
		horizontal = false;
	}

	/* get the end point tile positions */
	endTilePositions.first = BWAPI::TilePosition(endPoints.first);
	endTilePositions.second = BWAPI::TilePosition(endPoints.second);

	/* check to see if same tile, this is OK */
	if (endTilePositions.first == endTilePositions.second) {
		tilePositions.push_back(endTilePositions.first);
		/* nothing else to do */
		return;
	}

	/* establish the start tile position and move down or right */
	if (yMatch) { // move on x

		right = true;

		if (endTilePositions.first.x() < endTilePositions.second.x()) {
			startTile = &endTilePositions.first;
			endTile = &endTilePositions.second;
		}
		else {
			startTile = &endTilePositions.second;
			endTile = &endTilePositions.first;
		}
	}
	else { // move on y

		down = true;

		if (endTilePositions.first.y() < endTilePositions.second.y()) {
			startTile = &endTilePositions.first;
			endTile = &endTilePositions.second;
		}
		else {
			startTile = &endTilePositions.second;
			endTile = &endTilePositions.first;
		}
	}

	/* add tiles in order */
	tilePositions.push_back(*startTile);
	currTile = *startTile;

	while (currTile != *endTile) {

		if (right) {
			currTile.x() += 1;
		}
		else {
			currTile.y() += 1;
		}
		tilePositions.push_back(currTile);
	}
	// endTile will already be added, don't add again!

	// set lengths
	lengthTiles = (int) tilePositions.size();
	lengthPixels =  lengthTiles * 32; 

	// add walk tiles 
	// get start and end 
	// 32px/8px = 4
	startWalkTile.x() = tilePositions.front().x() * 4;
	startWalkTile.y() = tilePositions.front().y() * 4;
	currentWalkTile = startWalkTile;
	walkTiles.reserve(tilePositions.size() * 4);
	// 1*4=4, 2*4=8, so 1*4+3=7, just before next tile

	for (int m = 0; m < (int) tilePositions.size() * 4; ++m) {
		walkTiles.push_back(currentWalkTile);
		if (horizontal) {
			currentWalkTile.x() += 1;
		}
		else {
			currentWalkTile.y() += 1;
		}
	}
}

EnhancedSide::~EnhancedSide(void)
{
}

pair<BWAPI::Position, BWAPI::Position> EnhancedSide::getEndPoints()
{
	return endPoints;
}
vector<BWAPI::TilePosition> EnhancedSide::getTilePositions()
{
	return tilePositions;
}
vector<BWAPI::Position> EnhancedSide::getWalkTiles(void) 
{
	return walkTiles;
}
EnhancedSide::Orientation EnhancedSide::getOrientation(void)
{
	return orientation;
}
int EnhancedSide::getLengthPixels(void)
{
	return lengthPixels;
}
int EnhancedSide::getLengthTiles(void)
{
	return lengthTiles;
}
bool EnhancedSide::isHorizontal(void)
{
	return horizontal;
}
// returns length not covered / overlapping and if not zero sets the leftover
// sides not covered in sidesNotCovered, unless NULL, then only returns the 
// length not covered
int EnhancedSide::checkCoverage(EnhancedSide coveringSide, vector<EnhancedSide> * sidesNotCovered)
{

	int lengthNotCovered;
	int xLeft, xRight, xCovLeft, xCovRight;
	int yTop, yBottom, yCovTop, yCovBottom;
	int diff;
	pair<BWAPI::Position, BWAPI::Position> newEndPoints;
	bool getSides;
	bool nothingCovered;

	// init to max
	lengthNotCovered = this->lengthPixels;

	// must be parallel
	if (coveringSide.horizontal != horizontal) {
		return lengthNotCovered;
	}

	// init
	getSides = true;
	nothingCovered = false;
	// lengths added on as found
	lengthNotCovered = 0;

	if (sidesNotCovered == NULL) {
		getSides = false;
	}

	xLeft = endPoints.first.x();
	xRight = endPoints.second.x();
	xCovLeft = coveringSide.endPoints.first.x();
	xCovRight = coveringSide.endPoints.second.x();

	yTop = endPoints.first.y();
	yBottom = endPoints.second.y();
	yCovTop = coveringSide.endPoints.first.y();
	yCovBottom = coveringSide.endPoints.second.y();

	// if horizontal compare x
	if (horizontal) {
		// check left end
		if (xCovLeft > xLeft) {
			if (xCovLeft < xRight) {
				diff = xCovLeft - xLeft;
				lengthNotCovered += diff;
				if (getSides) {
					newEndPoints.first.x() = xLeft;
					newEndPoints.second.x() = xCovLeft;
					// use y val and orientation of the side trying to cover
					newEndPoints.first.y() = yTop;
					newEndPoints.second.y() = yTop;
					sidesNotCovered->push_back(EnhancedSide(newEndPoints, orientation));
				}
			}
			else {
				nothingCovered = true;
			}
		}

		// check right end
		if (xCovRight < xRight) {
			// do they overlap at all?
			if (xCovRight > xLeft) {
				diff = xRight - xCovRight; 
				lengthNotCovered += diff;
				if (getSides) {
					newEndPoints.first.x() = xCovRight;
					newEndPoints.second.x() = xRight;
					// use y val and orientation of the side trying to cover
					newEndPoints.first.y() = yTop;
					newEndPoints.second.y() = yTop;
					sidesNotCovered->push_back(EnhancedSide(newEndPoints, orientation));
				}
			}
			else {
				nothingCovered = true;
			}
		}
	}

	// if vertical compare y
	else {
		// check top end
		if (yCovTop > yTop) {
			if (yCovTop < yBottom) {
				diff = yCovTop - yTop;
				lengthNotCovered += diff;
				if (getSides) {
					newEndPoints.first.y() = yTop;
					newEndPoints.second.y() = yCovTop;
					// use x val and orientation of the side trying to cover
					newEndPoints.first.x() = xLeft;
					newEndPoints.second.x() = xLeft;
					sidesNotCovered->push_back(EnhancedSide(newEndPoints, orientation));
				}
			}
			else {
				nothingCovered = true;
			}
		}

		// check bottom end
		if (yCovBottom < yBottom) {
			if (yCovBottom > yTop) {
				diff = yBottom - yCovBottom;
				lengthNotCovered += diff;
				if (getSides) {
					newEndPoints.first.y() = yCovBottom;
					newEndPoints.second.y() = yBottom;
					// use x val and orientation of the side trying to cover
					newEndPoints.first.x() = xLeft;
					newEndPoints.second.x() = xLeft;
					sidesNotCovered->push_back(EnhancedSide(newEndPoints, orientation));
				}
			}
			else {
				nothingCovered = true;
			}
		}
	}

	if (nothingCovered) {
		lengthNotCovered = this->lengthPixels;
		if (getSides) {
			sidesNotCovered->push_back(*this);
		}
	}

	return lengthNotCovered;
}

// finds minimum gap between a side and a wall by searching for buildable tiles
// returns the minimum number of tiles found
// It's a fine grained assessment, searching with canBuildHere() should be done
// before calling this function.
// This function will let you know if you need to shift all your build places 
// as a group or it will tell you if this arrangement is better than another.
//  bool isWalkable(int walkX, int walkY);
//Returns true if the specified walk tile is walkable. The values of x and y are in walk tile 
//coordinates (different from build tile coordinates). Note that this just uses the static map data. 
//You will also need to make sure no ground units are on the coresponding build tile to see if its 
//currently walkable. To do this, see getUnitsOnTile.
//Keep in mind that a "walk tile" is a 8x8 pixel tile, and 16 walk tiles make up a build tile.
int EnhancedSide::checkMinGap()
{
	//vector<BWAPI::TilePosition> tiles = this->getTilePositions();
	vector<BWAPI::Position> walkTiles = getWalkTiles();
	//BWAPI::TilePosition currentTile;
	BWAPI::Position currentWalkTile;
	//int tileLength = (int)tiles.size();
	int tileLength = (int) walkTiles.size();
	int xDir, yDir;
	int currentNumTiles;
	int minNumTiles = 0;
	//bool unbuildableTileNotFound;
	bool unwalkableTileNotFound;

	// get xDir and yDir based on orientation
	if (orientation == EnhancedSide::left) {
		xDir = -1;
		yDir = 0;
	}
	else if (orientation == EnhancedSide::top) {
		xDir = 0;
		yDir = -1;
	}
	else if (orientation == EnhancedSide::right) {
		xDir = +1;
		yDir = 0;
	}
	else if (orientation == EnhancedSide::bottom) {
		xDir = 0;
		yDir = +1;
	}

	for (int m = 0; m < tileLength; ++m) {
		currentWalkTile = walkTiles[m];
		// reset
		unwalkableTileNotFound = true;
		currentNumTiles = 0;

		while (unwalkableTileNotFound) {
			// increment
			currentWalkTile.x() += xDir;
			currentWalkTile.y() += yDir;

			// check tile
			//if (BWAPI::Broodwar->isBuildable(currentWalkTile, true)) {
			if (BWAPI::Broodwar->isWalkable(currentWalkTile.x(), currentWalkTile.y())) {
				// add to this rounds gap
				currentNumTiles++;
			}
			else {
				// found the wall
				unwalkableTileNotFound = false;
				break;
			}
		}

		// check for minimum
		if (m == 0) {
			minNumTiles = currentNumTiles;
		}
		else if (currentNumTiles < minNumTiles) {
			minNumTiles = currentNumTiles;
		}
	}

	return minNumTiles;
}

// find max gap
int EnhancedSide::checkMaxGap()
{
	vector<BWAPI::Position> walkTiles = getWalkTiles();
	BWAPI::Position currentWalkTile;
	int tileLength = (int)walkTiles.size();
	int xDir, yDir;
	int currentNumTiles;
	int maxNumTiles = 0;
	bool unwalkableTileNotFound;

	// get xDir and yDir based on orientation
	if (orientation == EnhancedSide::left) {
		xDir = -1;
		yDir = 0;
	}
	else if (orientation == EnhancedSide::top) {
		xDir = 0;
		yDir = -1;
	}
	else if (orientation == EnhancedSide::right) {
		xDir = +1;
		yDir = 0;
	}
	else if (orientation == EnhancedSide::bottom) {
		xDir = 0;
		yDir = +1;
	}

	for (int m = 0; m < tileLength; ++m) {
		currentWalkTile = walkTiles[m];
		// reset
		unwalkableTileNotFound = true;
		currentNumTiles = 0;

		while (unwalkableTileNotFound) {
			// increment
			currentWalkTile.x() += xDir;
			currentWalkTile.y() += yDir;

			// check tile
			if (BWAPI::Broodwar->isWalkable(currentWalkTile.x(), currentWalkTile.y())) {
				// add to this rounds gap
				currentNumTiles++;
			}
			else {
				// found the wall
				unwalkableTileNotFound = false;
				break;
			}
		}

		// check for minimum
		if (m == 0) {
			maxNumTiles = currentNumTiles;
		}
		else if (currentNumTiles > maxNumTiles) {
			maxNumTiles = currentNumTiles;
		}
	}

	return maxNumTiles;
}

/* draws a line */
void EnhancedSide::drawSide(BWAPI::Color color)
{
	BWAPI::Broodwar->drawLineMap(endPoints.first.x(), endPoints.first.y(), 
		endPoints.second.x(), endPoints.second.y(), color); 
}
/* draws all the individual tiles for that side */
void EnhancedSide::drawTiles(BWAPI::Color color)
{
	EnhancedUI * eui = new EnhancedUI();
	for (int m = 0; m < (int) tilePositions.size(); ++m) {
		eui->drawTilePosition(tilePositions[m], color);
	}
	delete(eui);
}
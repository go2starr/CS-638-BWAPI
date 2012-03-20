
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
	/* endTile will already be added, don't add again! */

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
EnhancedSide::Orientation EnhancedSide::getOrientation(void)
{
	return orientation;
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

	int lengthNotCovered = 0;
	int xLeft, xRight, xCovLeft, xCovRight;
	int yTop, yBottom, yCovTop, yCovBottom;
	int diff;
	pair<BWAPI::Position, BWAPI::Position> newEndPoints;
	bool getSides;

	// must be parallel
	if (coveringSide.horizontal != horizontal) {
		return -1;
	}

	getSides = true;

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
	yCovBottom = coveringSide.endPoints.second.x();

	// if horizontal compare x
	if (horizontal) {
		// check left end
		if (xCovLeft > xLeft) {
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

		// check right end
		if (xCovRight < xRight) {
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
	}

	// if vertical compare y
	else {
		// check top end
		if (yCovTop > yTop) {
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

		// check bottom end
		if (yCovBottom < yBottom) {
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
	}

	return lengthNotCovered;
}

// finds minimum gap between a side and a wall by searching for buildable tiles
// returns the minimum number of tiles found
// It's a fine grained assessment, searching with canBuildHere() should be done
// before calling this function.
// This function will let you know if you need to shift all your build places 
// as a group or it will tell you if this arrangement is better than another.
int EnhancedSide::checkGap()
{
	vector<BWAPI::TilePosition> tiles = this->getTilePositions();
	BWAPI::TilePosition currentTile;
	int tileLength = (int)tiles.size();
	int xDir, yDir;
	int currentNumTiles;
	int minNumTiles = 0;
	bool unbuildableTileNotFound;

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
		currentTile = tiles[m];
		// reset
		unbuildableTileNotFound = true;
		currentNumTiles = 0;

		while (unbuildableTileNotFound) {
			// increment
			currentTile.x() += xDir;
			currentTile.y() += yDir;

			// check tile
			if (BWAPI::Broodwar->isBuildable(currentTile, true)) {
				// add to this rounds gap
				currentNumTiles++;
			}
			else {
				// found the wall
				unbuildableTileNotFound = false;
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
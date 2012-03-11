
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
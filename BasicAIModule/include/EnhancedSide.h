#pragma once
/*
 * EnhancedSide.h
 */
#include <BWAPI.h>
#include <BWTA.h>

#include <EnhancedUI.h>

#include <assert.h>

using std::pair;
using std::vector;

class EnhancedSide
{
public:
	enum Orientation {left, top, right, bottom};
	EnhancedSide(pair<BWAPI::Position, BWAPI::Position> endPoints, Orientation orientation);
	~EnhancedSide(void);
	pair<BWAPI::Position, BWAPI::Position> getEndPoints();
	vector<BWAPI::TilePosition> getTilePositions();
	Orientation getOrientation(void);
	void drawSide(BWAPI::Color color);
	void drawTiles(BWAPI::Color color);

private:
	Orientation orientation;
	pair<BWAPI::Position, BWAPI::Position> endPoints;
	pair<BWAPI::TilePosition, BWAPI::TilePosition> endTilePositions;
	vector<BWAPI::TilePosition> tilePositions;
};

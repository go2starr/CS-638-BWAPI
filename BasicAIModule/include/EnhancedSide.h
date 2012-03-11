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
	EnhancedSide(pair<BWAPI::Position, BWAPI::Position> endPoints);
	~EnhancedSide(void);
	pair<BWAPI::Position, BWAPI::Position> getEndPoints();
	vector<BWAPI::TilePosition> getTilePositions();
	void drawSide(BWAPI::Color color);
	void drawTiles(BWAPI::Color color);

private:
	pair<BWAPI::Position, BWAPI::Position> endPoints;
	pair<BWAPI::TilePosition, BWAPI::TilePosition> endTilePositions;
	vector<BWAPI::TilePosition> tilePositions;
};

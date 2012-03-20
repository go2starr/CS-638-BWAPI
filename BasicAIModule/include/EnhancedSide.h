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
	pair<BWAPI::Position, BWAPI::Position> getEndPoints(void);
	vector<BWAPI::TilePosition> getTilePositions(void);
	Orientation getOrientation(void);
	bool isHorizontal(void);
	// returns length not covered / overlapping and if not zero sets the leftover
	// sides not covered in sidesNotCovered. 
	int checkCoverage(EnhancedSide coveringSide, vector<EnhancedSide> * sidesNotCovered);
	// finds minimum gap between a side and a wall by searching for buildable tiles
	// returns the minimum number of tiles found
	int checkGap(void);
	void drawSide(BWAPI::Color color);
	void drawTiles(BWAPI::Color color);

private:
	Orientation orientation;
	pair<BWAPI::Position, BWAPI::Position> endPoints;
	pair<BWAPI::TilePosition, BWAPI::TilePosition> endTilePositions;
	vector<BWAPI::TilePosition> tilePositions;
	// is this a vertical side or not
	bool horizontal;
};

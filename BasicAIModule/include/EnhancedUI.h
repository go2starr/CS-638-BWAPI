#pragma once
#include <vector>
#include <BWAPI.h>
#include <BWTA.h>
//#include <BWSAL.h>

using namespace std;

class EnhancedUI
{
public:
	void update() const;
	void drawBoxAtTilePositionToSize(BWAPI::TilePosition tpos, int width, int height);
	int getMinTileSize(int pixels);
	int getMaxTileSize(int pixels);
	BWAPI::Position getTilePositionCenter(BWAPI::TilePosition);
	void drawRegionBoundingBox(const BWTA::Region * region);
	void getRegionBoundingTilePositions(const BWTA::Region * region, 
		BWAPI::TilePosition & topLeftTP, 
		BWAPI::TilePosition & topRightTP, 
		BWAPI::TilePosition & bottomRightTP, 
		BWAPI::TilePosition & bottomLeftTP);
	void drawPolygonFromRegion(BWTA::Region * region, BWAPI::Color color);
/*
	void addFoundTilePositionsToSize(vector<BWAPI::TilePosition> & tPositions, 
											 BWAPI::TilePosition upperLeftTPos, 
											 int width, 
											 int height);

	bool hasTilePosition(vector<BWAPI::TilePosition> & tPositions, 
											 BWAPI::TilePosition tPos);


	void drawBoxToSizeInBoundingTiles(BWAPI::TilePosition * topLeftTP, 
		BWAPI::TilePosition * topRightTP, 
		BWAPI::TilePosition * bottomRightTP, 
		BWAPI::TilePosition * bottomLeftTP,
		int tileWidth, int tileHeight);
*/
	void drawTilePosition(BWAPI::TilePosition tPos, BWAPI::Color color);

private:
	/* Terran Supply Depot positioning storage */
	vector<BWAPI::TilePosition> foundSDTPos;
	vector<BWAPI::TilePosition> buildSDTPos;

	void drawBases() const;
	void drawTerrain() const;
	//  void drawProgress() const;
	//  void drawProgressBar(BWAPI::Position pos, double progressFaction, BWAPI::Color innerBar = BWAPI::Colors::Green) const;
};


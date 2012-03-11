#pragma once
/*
 * EnhancedChokepoint.h
 */
#include <BWAPI.h>
#include <BWTA.h>

#include <EnhancedUI.h>
#include <EnhancedSide.h>

#include <assert.h>

using std::pair;
using std::vector;

class EnhancedChokepoint
{
public:
	EnhancedChokepoint(BWTA::Chokepoint * chokepoint);
	~EnhancedChokepoint(void);
	/* BWTA Chokepoint functions */
	const pair<BWTA::Region *, BWTA::Region *> & getRegions();
	const pair<BWAPI::Position, BWAPI::Position> & getSides();
	BWAPI::Position getCenter();
	double getWidth();

	/* enhanced functionality */
	bool isPositionInPolygon(BWAPI::Position searchPos, BWTA::Polygon poly);
	vector<EnhancedSide> & getBoundingBoxSidesForRegion(BWTA::Region * region);
	void drawTilePositions(void);
	void drawBoundingBox(void);
	void drawBoundingBoxSidesForRegion(BWTA::Region * region, BWAPI::Color color);
	

private:
	BWTA::Chokepoint * cp;
	/* tiles for chokepoint line sides and center positions */
	pair<BWAPI::TilePosition, BWAPI::TilePosition> tPosSides;
	BWAPI::TilePosition tPosCenter;
	/* bounding box coords */
	int xLeft, xRight, yTop, yBottom;
	/* pair a region to sides of bounding box, there are only 2 regions per chokepoint */
	pair<BWTA::Region * , vector<EnhancedSide>> regionASides;
	pair<BWTA::Region * , vector<EnhancedSide>> regionBSides;

	bool isSideInRegion(pair<BWAPI::Position, BWAPI::Position> endPoints, BWTA::Region * region);
};

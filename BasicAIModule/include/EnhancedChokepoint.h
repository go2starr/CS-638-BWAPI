#pragma once
/*
 * EnhancedChokepoint.h
 */
#include <BWAPI.h>
#include <BWTA.h>

#include <EnhancedUI.h>

#include <assert.h>


class EnhancedChokepoint
{
public:
	EnhancedChokepoint(BWTA::Chokepoint * cp);
	~EnhancedChokepoint(void);

	const std::pair<BWTA::Region *, BWTA::Region *> & getRegions();
	const std::pair<BWAPI::Position, BWAPI::Position> & getSides();
	BWAPI::Position getCenter();
	double getWidth();

	void drawTilePositions(void);
	void EnhancedChokepoint::drawBoundingBox(void);

private:
	BWTA::Chokepoint * cp;
	std::pair<BWAPI::TilePosition, BWAPI::TilePosition> tPosSides;
	BWAPI::TilePosition tPosCenter;
	int xLeft, xRight, yTop, yBottom;
};

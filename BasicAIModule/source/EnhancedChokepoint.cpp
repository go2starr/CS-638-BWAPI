/*
 * EnhancedChokepoint.cpp
 */
#include "EnhancedChokepoint.h"

#include <BWAPI.h>
#include <BWTA.h>

#include <assert.h>

using std::pair;


EnhancedChokepoint::EnhancedChokepoint(BWTA::Chokepoint * cp)
{
	pair<BWAPI::Position, BWAPI::Position> sides; 

	assert(cp);
	this->cp = cp;

	sides = cp->getSides();

	/* get tile positions for center & sides */
	tPosCenter = BWAPI::TilePosition(cp->getCenter());
	tPosSides.first = BWAPI::TilePosition(sides.first);
	tPosSides.second = BWAPI::TilePosition(sides.second);

	/* get bounding box in position coords by tile positions */

	if(tPosSides.first.x() <= tPosSides.second.x()) {
		xLeft = tPosSides.first.x() * 32;
		xRight = (tPosSides.second.x() + 1) * 32;
	}
	else {
		xLeft = tPosSides.second.x() * 32;
		xRight = (tPosSides.first.x() + 1) * 32;
	}

	if(tPosSides.first.y() <= tPosSides.second.y()) {
		yTop = tPosSides.first.y() * 32;
		yBottom = (tPosSides.second.y() + 1) * 32;
	}
	else {
		yTop = tPosSides.second.y() * 32;
		yBottom = (tPosSides.first.y() + 1) * 32;
	}
}

EnhancedChokepoint::~EnhancedChokepoint()
{
}

/* wrapper Chokepoint functions */
const pair<BWTA::Region *, BWTA::Region *> & EnhancedChokepoint::getRegions() 
{
	return cp->getRegions();
}

const std::pair<BWAPI::Position, BWAPI::Position> & EnhancedChokepoint::getSides() 
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
void EnhancedChokepoint::drawTilePositions() 
{
	EnhancedUI * eui = new EnhancedUI();
	BWAPI::Color color = BWAPI::Colors::Purple;
	eui->drawTilePosition(this->tPosCenter, color);
	eui->drawTilePosition(this->tPosSides.first, color);
	eui->drawTilePosition(this->tPosSides.second, color);
	delete eui;
}

void EnhancedChokepoint::drawBoundingBox(void)
{
	BWAPI::Broodwar->drawBoxMap(xLeft, yTop, xRight, yBottom, BWAPI::Colors::White, false);
}
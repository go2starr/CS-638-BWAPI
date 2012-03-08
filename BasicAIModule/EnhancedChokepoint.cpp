#include "EnhancedChokepoint.h"

EnhancedChokepoint::EnhancedChokepoint(BWTA::Chokepoint * cp)
{
	pair<BWAPI::Position, BWAPI::Position> sides; 

	assert(cp);
	this->cp = cp;

	sides = cp->getSides();

	/* get tile positions for center & sides */
	this->tPosCenter = BWAPI::TilePosition(cp->getCenter());
	this->tPosSides.first = BWAPI::TilePosition(sides.first);
	this->tPosSides.second = BWAPI::TilePosition(sides.second);

	/* get bounding box in position coords by tile positions */

	if(this->tPosSides.first.x() <= this->tPosSides.second.x()) {
		this->xLeft = this->tPosSides.first.x() * 32;
		this->xRight = (this->tPosSides.second.x() + 1) * 32;
	}
	else {
		this->xLeft = this->tPosSides.second.x() * 32;
		this->xRight = (this->tPosSides.first.x() + 1) * 32;
	}

	if(this->tPosSides.first.y() <= this->tPosSides.second.y()) {
		this->yTop = this->tPosSides.first.y() * 32;
		this->yBottom = (this->tPosSides.second.y() + 1) * 32;
	}
	else {
		this->yTop = this->tPosSides.second.y() * 32;
		this->yBottom = (this->tPosSides.first.y() + 1) * 32;
	}
}

EnhancedChokepoint::~EnhancedChokepoint(void)
{
}

/* wrapper Chokepoint functions */
const std::pair<BWTA::Region *, BWTA::Region *> & EnhancedChokepoint::getRegions() 
{
	return this->cp->getRegions();
}

const std::pair<BWAPI::Position, BWAPI::Position> & EnhancedChokepoint::getSides() 
{
	return this->cp->getSides();
}
BWAPI::Position EnhancedChokepoint::getCenter() 
{
	return this->cp->getCenter();
}
double EnhancedChokepoint::getWidth() 
{
	return this->cp->getWidth();
}

/* enhanced functions */
void EnhancedChokepoint::drawTilePositions(void) 
{
	EnhancedUI * eui = new EnhancedUI();
	BWAPI::Color color = BWAPI::Colors::Purple;
	eui->drawTilePosition(this->tPosCenter, color);
	eui->drawTilePosition(this->tPosSides.first, color);
	eui->drawTilePosition(this->tPosSides.second, color);
	delete(eui);
}

void EnhancedChokepoint::drawBoundingBox(void)
{
	BWAPI::Broodwar->drawBoxMap(this->xLeft, this->yTop, this->xRight, this->yBottom, BWAPI::Colors::White, false);
}
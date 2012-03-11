/*
* EnhancedChokepoint.cpp
*/
#include "EnhancedChokepoint.h"

#include <BWAPI.h>
#include <BWTA.h>

#include <assert.h>

using std::pair;
using std::vector;


EnhancedChokepoint::EnhancedChokepoint(BWTA::Chokepoint * chokepoint)
{
	pair<BWAPI::Position, BWAPI::Position> sides;
	/* corners */
	BWAPI::Position topLeft, topRight, bottomLeft, bottomRight;
	/* sides */
	pair<BWAPI::Position, BWAPI::Position> leftSide, topSide, rightSide, bottomSide;
	/* regions */
	pair<BWTA::Region *, BWTA::Region *> regions;

	assert(chokepoint);
	cp = chokepoint;

	regions = cp->getRegions();
	sides = cp->getSides();

	/* get tile positions for center & sides */
	tPosCenter = BWAPI::TilePosition(cp->getCenter());
	tPosSides.first = BWAPI::TilePosition(sides.first);
	tPosSides.second = BWAPI::TilePosition(sides.second);

	/* get bounding box in position coords by tile positions */

	if(tPosSides.first.x() <= tPosSides.second.x()) {
		xLeft = tPosSides.first.x() * 32;
		xRight = ((tPosSides.second.x() + 1) * 32) - 1;
	}
	else {
		xLeft = tPosSides.second.x() * 32;
		xRight = ((tPosSides.first.x() + 1) * 32) - 1;
	}

	if(tPosSides.first.y() <= tPosSides.second.y()) {
		yTop = tPosSides.first.y() * 32;
		yBottom = ((tPosSides.second.y() + 1) * 32) - 1;
	}
	else {
		yTop = tPosSides.second.y() * 32;
		yBottom = ((tPosSides.first.y() + 1) * 32) - 1;
	}
	
	/* set corner positions */
	topLeft = BWAPI::Position(xLeft, yTop);
	topRight = BWAPI::Position(xRight, yTop);
	bottomLeft = BWAPI::Position(xLeft, yBottom);
	bottomRight = BWAPI::Position(xRight, yBottom);

	/* set sides, first position must be less
	 * than the second (x or y), for searching purposes,
	 * ie, only search right or down
	 */
	leftSide.first = topLeft;
	leftSide.second = bottomLeft;
	rightSide.first = topRight;
	rightSide.second = bottomRight;
	topSide.first = topLeft;
	topSide.second = topRight;
	bottomSide.first = bottomLeft;
	bottomSide.second = bottomRight;

	/* search for sides in region A */
	regionASides.first = regions.first;
	if (isSideInRegion(leftSide, regions.first)) {
		regionASides.second.push_back(EnhancedSide(leftSide));
	}
	if (isSideInRegion(topSide, regions.first)) {
		regionASides.second.push_back(EnhancedSide(topSide));
	}
	if (isSideInRegion(rightSide, regions.first)) {
		regionASides.second.push_back(EnhancedSide(rightSide));
	}
	if (isSideInRegion(bottomSide, regions.first)) {
		regionASides.second.push_back(EnhancedSide(bottomSide));
	}

	
	/* serach for sides in region B */
	regionBSides.first = regions.second;
	if (isSideInRegion(leftSide, regions.second)) {
		regionBSides.second.push_back(EnhancedSide(leftSide));
	}
	if (isSideInRegion(topSide, regions.second)) {
		regionBSides.second.push_back(EnhancedSide(topSide));
	}
	if (isSideInRegion(rightSide, regions.second)) {
		regionBSides.second.push_back(EnhancedSide(rightSide));
	}
	if (isSideInRegion(bottomSide, regions.second)) {
		regionBSides.second.push_back(EnhancedSide(bottomSide));
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

const pair<BWAPI::Position, BWAPI::Position> & EnhancedChokepoint::getSides() 
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

// http://alienryderflex.com/polygon/
//  Globals which should be set before calling this function:
//
//  int    polySides  =  how many corners the polygon has
//  float  polyX[]    =  horizontal coordinates of corners
//  float  polyY[]    =  vertical coordinates of corners
//  float  x, y       =  point to be tested
//
//  (Globals are used in this example for purposes of speed.  Change as
//  desired.)
//
//  The function will return YES if the point x,y is inside the polygon, or
//  NO if it is not.  If the point is exactly on the edge of the polygon,
//  then the function may return YES or NO.
//
//  Note that division by zero is avoided because the division is protected
//  by the "if" clause which surrounds it.
bool EnhancedChokepoint::isPositionInPolygon(BWAPI::Position searchPos, BWTA::Polygon poly){

int numPositions = (int) poly.size();
int polySides = numPositions;
vector<float>polyX;
vector<float>polyY;
float x, y;
int      i, j;
bool  oddNodes = false;
BWAPI::Position pos;

polyX.reserve(numPositions);
polyY.reserve(numPositions);

/* get position values from polygon */
for (int m = 0; m < numPositions; ++m) {
	pos = poly[m];
	polyX.push_back((float) pos.x());
	polyY.push_back((float) pos.y());
}

/* get position to search for */
x = (float) searchPos.x();
y = (float) searchPos.y();


/* init j */
j = polySides - 1;

  for (i=0; i<polySides; i++) {
    if ((polyY[i]< y && polyY[j]>=y
    ||   polyY[j]< y && polyY[i]>=y)
    &&  (polyX[i]<=x || polyX[j]<=x)) {
      oddNodes^=(polyX[i]+(y-polyY[i])/(polyY[j]-polyY[i])*(polyX[j]-polyX[i])<x); }
    j=i; }

  return oddNodes; 
}


/*
* first position must be less then the second, ie always search down or right
* does not check the corner points, only the inside points
*/
bool EnhancedChokepoint::isSideInRegion(pair<BWAPI::Position, BWAPI::Position> endPoints, BWTA::Region * region)
{
	BWAPI::Position pos;
	BWTA::Polygon poly;
	/* fine / coarse grain size, 8 = 1/4 of a tile length */
	const int grainLengthForSearch = 8;

	assert(region);

	poly = region->getPolygon();

	if (endPoints.first.x() == endPoints.second.x()) {
		/* only search down */
		assert(endPoints.first.y() <= endPoints.second.y());
		/* search along y, x is constant */
		pos = BWAPI::Position(endPoints.first.x(), endPoints.first.y() + grainLengthForSearch);

		while (pos.y() < endPoints.second.y()) {
			// BWTA isInside() is broken, use ours
	//		if (poly.isInside(pos)) {
			if (isPositionInPolygon(pos, poly)) {
				return true;
			}
			pos.y() += grainLengthForSearch;
		}
	}
	else {
		/* only search right */
		assert(endPoints.first.x() <= endPoints.second.x());
		/* search along x, y is constant */
		pos = BWAPI::Position(endPoints.first.x() + grainLengthForSearch, endPoints.first.y());

		while (pos.x() < endPoints.second.x()) {
			// BWTA isInside() is broken, use ours
			//if (poly.isInside(pos)) {
			if (isPositionInPolygon(pos, poly)) {
				return true;
			}
			pos.x() += grainLengthForSearch;
		}
	}
	return false;
}
vector<EnhancedSide> & EnhancedChokepoint::getBoundingBoxSidesForRegion(BWTA::Region * region)
{

	assert(region);
	
	if (region == regionASides.first) {
		return regionASides.second;
	}
	
	return regionBSides.second;
}
void EnhancedChokepoint::drawBoundingBoxSidesForRegion(BWTA::Region * region, BWAPI::Color color)
{
	assert(region);

	if (region == regionASides.first) {
		for (int m = 0; m < (int) regionASides.second.size(); ++m) {
			regionASides.second[m].drawSide(color);
		}
	}
	else if (region == regionBSides.first) {
		for (int m = 0; m < (int) regionBSides.second.size(); ++m) {
			regionBSides.second[m].drawSide(color);
		}
	}
}
/* draws tile positions for the sides, and center */
void EnhancedChokepoint::drawTilePositions() 
{
	EnhancedUI * eui = new EnhancedUI();
	BWAPI::Color color = BWAPI::Colors::Purple;
	eui->drawTilePosition(this->tPosSides.first, color);
	eui->drawTilePosition(this->tPosSides.second, color);
	/* draw this last and in a different color */
	eui->drawTilePosition(this->tPosCenter, BWAPI::Colors::Yellow);
	delete eui;
}

void EnhancedChokepoint::drawBoundingBox(void)
{
	BWAPI::Broodwar->drawBoxMap(xLeft, yTop, xRight, yBottom, BWAPI::Colors::White, false);
}
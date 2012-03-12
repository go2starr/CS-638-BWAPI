#pragma once
/*
* TacticalBuildingPlacer.cpp
*/

#include <TacticalBuildingPlacer.h>
#include <BWSAL/BFSBuildingPlacer.h>

TacticalBuildingPlacer::TacticalBuildingPlacer()
{

	BWTA::BaseLocation * baseLoc;
	BWTA::Region * region;
	set<BWTA::Chokepoint *> startLocChokepoints;
	set<BWTA::Chokepoint *>::iterator cpi;
	pair<BWTA::Region *, vector<EnhancedChokepoint>> regionChokepoints;

	/* get enhanced chokepoints for start location */
	baseLoc = BWTA::getStartLocation(BWAPI::Broodwar->self());
	region = baseLoc->getRegion();
	startLocChokepoints = region->getChokepoints();

	/* set base region */
	regionChokepoints.first = region;

	/* set chokepoints for base region */
	for (cpi = startLocChokepoints.begin(); cpi != startLocChokepoints.end(); ++cpi) {
		EnhancedChokepoint ecPoint(*cpi);
		regionChokepoints.second.push_back(ecPoint);
	}

	/* save combo */
	regionsToChokepoints.push_back(regionChokepoints);
}

TacticalBuildingPlacer::~TacticalBuildingPlacer(void)
{
}

void TacticalBuildingPlacer::analyze(void)
{


}

void TacticalBuildingPlacer::draw(void)
{
	BWTA::Region * region;
	vector<EnhancedChokepoint> * chokepoints;

	for (int m = 0; m < (int) regionsToChokepoints.size(); ++m) {

		region = regionsToChokepoints[m].first;
		chokepoints = &regionsToChokepoints[m].second;

		/* draw for enhanced chokepoints */
		for (int x = 0; x < (int) chokepoints->size(); ++x) {
			/* draw tiles for the sides and center of chokepoint line */
			chokepoints[0][x].drawTilePositions();
			/* leave out for now, sides can be hard to see with this on */
			//ecPoints[x].drawBoundingBox();
			chokepoints[0][x].drawBoundingBoxSidesForRegion(region, BWAPI::Colors::Red);
		}
	}
}

void TacticalBuildingPlacer::update(void)
{
}

BWAPI::TilePosition TacticalBuildingPlacer::reserveBuildLocation(BWAPI::UnitType unitType, BWAPI::TilePosition seedLocation, BWAPI::Unit *builder)
{
	BWAPI::TilePosition loc;

	loc = buildingPlacer.findBuildLocation(BWSAL::ReservedMap::getInstance(), unitType, seedLocation, builder);

	BWSAL::ReservedMap::getInstance()->reserveTiles(loc, unitType, unitType.tileWidth(), unitType.tileHeight());

	return loc;
}
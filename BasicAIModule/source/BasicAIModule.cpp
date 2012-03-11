/*
 * BasicAIModule.cpp
 */
#include "BasicAIModule.h"
#include "Strategizer.h"

#include <BWAPI.h>
#include <BWSAL.h>
#include <BWTA.h>

#include <EnhancedChokepoint.h>

#include <string>
#include <vector>

using namespace BWAPI;
using std::string;
using std::vector;

/* chokepoint analysis */
vector<EnhancedChokepoint> ecPoints;


/* 
 * onStart()
 *
 * Called by the game when a new match using this AI module begins
 */
void BasicAIModule::onStart()
{
	BWTA::BaseLocation * baseLoc;
	BWTA::Region * region;
	set<BWTA::Chokepoint *> startLocChokepoints;
	set<BWTA::Chokepoint *>::iterator cpi;

	/* set up BWTA */
	BWTA::readMap();
	BWTA::analyze();
//	BWSAL::resetLog();

	Broodwar->sendText("UW-Madison : CS638 Software Engineering - Brood War AI");

	Strategizer::instance().onMatchStart();

	enhancedUI = new EnhancedUI();

	/* get enhanced chokepoints for start location */
	baseLoc = BWTA::getStartLocation(Broodwar->self());
	region = baseLoc->getRegion();
	startLocChokepoints = region->getChokepoints();
	for (cpi = startLocChokepoints.begin(); cpi != startLocChokepoints.end(); ++cpi) {
		EnhancedChokepoint ecPoint(*cpi);
		ecPoints.push_back(ecPoint);
	}
}

/* 
 * onEnd()
 *
 * Called by the game when a match using this AI module ends
 */
void BasicAIModule::onEnd(bool isWinner)
{
}

/* 
 * onFrame()
 *
 * Called by the game on each frames
 */
void BasicAIModule::onFrame()
{
	BWTA::BaseLocation * baseLoc;
	BWTA::Region * region;

	/* where we live */
	baseLoc = BWTA::getStartLocation(Broodwar->self());
	/* get base region */
	region = baseLoc->getRegion();
	/* draw for all terrain */
	enhancedUI->update();
	/* base region get's a different color, easier to see */
	enhancedUI->drawPolygonFromRegion(region, BWAPI::Colors::Teal);

	/* additional UI */
	/* draw for enhanced chokepoints */
	for (int x = 0; x < (int) ecPoints.size(); ++x) {
		/* draw tiles for the sides and center of chokepoint line */
		ecPoints[x].drawTilePositions();
		/* leave out for now, sides can be hard to see with this on */
		//ecPoints[x].drawBoundingBox();
		ecPoints[x].drawBoundingBoxSidesForRegion(region, BWAPI::Colors::Red);
	}
	/* shows max width and height of base region */
	enhancedUI->drawRegionBoundingBox(region);

	/* update the Strategizer */
	Strategizer::instance().update();
}

/* 
 * onUnitDiscover()
 *
 */
void BasicAIModule::onUnitDiscover(Unit* unit)
{
	const string& name = unit->getType().getName();

	// This conditional is temporary to filter out all the 
	// resource and building units that BWTA finds.
	if( name == "Terran SCV" )
		Broodwar->sendText("Unit discovered: %s", name.c_str());
}

/* 
 * onUnitEvade()
 *
 */
void BasicAIModule::onUnitEvade(Unit* unit)
{

}

/* 
 * onUnitMorph()
 *
 */
void BasicAIModule::onUnitMorph(Unit* unit)
{

}

/* 
 * onUnitRenegade()
 *
 */
void BasicAIModule::onUnitRenegade(Unit* unit)
{

}

/* 
 * onUnitDestroy()
 *
 */
void BasicAIModule::onUnitDestroy(Unit* unit)
{

}

/* 
 * onSendText()
 *
 */
void BasicAIModule::onSendText(string text)
{

}

/* 
 * showPlayers()
 *
 */
void BasicAIModule::showPlayers()
{

}

/* 
 * showForces()
 *
 */
void BasicAIModule::showForces()
{

}


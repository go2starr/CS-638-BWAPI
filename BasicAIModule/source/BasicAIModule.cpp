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

	baseLoc = BWTA::getStartLocation(Broodwar->self());
	region = baseLoc->getRegion();

	enhancedUI->update();

	/* additional UI */
	/* draw for enhanced chokepoints */
	for (int x = 0; x < (int) ecPoints.size(); ++x) {
		ecPoints[x].drawTilePositions();
		ecPoints[x].drawBoundingBox();
	}
	/* for show */
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
	Broodwar->sendText("Unit discovered: %s", unit->getType().getName());
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


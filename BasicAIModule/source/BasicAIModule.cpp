/*
 * BasicAIModule.cpp
 */
#include "BasicAIModule.h"
#include "Strategizer.h"


#include <BWAPI.h>
#include <BWTA.h>

#include <string>
#include <vector>

using namespace BWAPI;
using std::string;
using std::vector;

/* 
 * onStart()
 *
 * Called by the game when a new match using this AI module begins
 */
void BasicAIModule::onStart()
{
	/* set up BWTA */
	BWTA::readMap();
	BWTA::analyze();
//	BWSAL::resetLog();

	/* requires BTWA::analyze(); */
	enhancedUI = new EnhancedUI();

	Broodwar->enableFlag(Flag::UserInput);

	Broodwar->sendText("UW-Madison : CS638 Software Engineering - Brood War AI");

	Strategizer::instance().onMatchStart();
}

/* 
 * onEnd()
 *
 * Called by the game when a match using this AI module ends
 */
void BasicAIModule::onEnd(bool isWinner)
{
    Strategizer::instance().onMatchEnd(isWinner);
}

/* 
 * onFrame()
 *
 * Called by the game on each frames
 */
void BasicAIModule::onFrame()
{
	/* draw for all terrain */
	enhancedUI->update();

	TacticalBuildingPlacer::instance().draw();

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
	Broodwar->sendText("Unit destroyed: %s", unit->getType().getName().c_str());
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


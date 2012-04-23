/*
 * BasicAIModule.cpp
 */
#include "BasicAIModule.h"
#include "Strategizer.h"
#include "DebugFlags.h"

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
    Strategizer::instance().onUnitDiscovered(unit);
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
	for(int i = 0; i < NUM_DEBUG_FLAGS; ++i)
	{
		const string& flag = DebugFlagStrings[i];
		if( text == flag )
		{
			const DebugFlag dflag = static_cast<DebugFlag>(i);
			DebugFlags::instance().toggleFlag(dflag);
			break;
		}
	}
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


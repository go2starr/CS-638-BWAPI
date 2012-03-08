#include "BasicAIModule.h"
#include "Strategizer.h"

#include <BWAPI.h>
#include <BWSAL.h>
#include <BWTA.h>

#include <string>

using namespace BWAPI;
using std::string;

/* 
 * onStart()
 *
 * Called by the game when a new match using this AI module begins
 */
void BasicAIModule::onStart()
{
    BWTA::readMap();
    BWTA::analyze();
//	BWSAL::resetLog();

	Broodwar->sendText("UW-Madison : CS638 Software Engineering - Brood War AI");

	Strategizer::instance().onMatchStart();

	enhancedUI = new EnhancedUI();
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
	enhancedUI->update();

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


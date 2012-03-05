#include <BWAPI.h>
#include <BWTA.h>
#include <Strategizer.h>
#include "BasicAIModule.h"


void BasicAIModule::onStart()
{
	Broodwar->sendText("Hello, CS638!");
	Strategizer::instance().onMatchStart();
}
void BasicAIModule::onEnd(bool isWinner)
{
}
void BasicAIModule::onFrame()
{
	Strategizer::instance().update();
}

void BasicAIModule::onUnitDiscover(BWAPI::Unit* unit)
{
	Broodwar->sendText("Unit discovered: %s", unit->getType().getName());
}

void BasicAIModule::onUnitEvade(BWAPI::Unit* unit)
{
}

void BasicAIModule::onUnitMorph(BWAPI::Unit* unit)
{
}

void BasicAIModule::onUnitRenegade(BWAPI::Unit* unit)
{
}

void BasicAIModule::onUnitDestroy(BWAPI::Unit* unit)
{
}

void BasicAIModule::onSendText(std::string text)
{
}

void BasicAIModule::showPlayers()
{
}

void BasicAIModule::showForces()
{
}


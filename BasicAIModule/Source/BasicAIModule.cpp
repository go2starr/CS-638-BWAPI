#include <BWAPI.h>
#include <BWTA.h>
#include "BasicAIModule.h"

void BasicAIModule::onStart()
{
  BWAPI::Broodwar->sendText("Hello, CS638!");
}
void BasicAIModule::onEnd(bool isWinner)
{
}
void BasicAIModule::onFrame()
{
}

void BasicAIModule::onUnitDiscover(BWAPI::Unit* unit)
{
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






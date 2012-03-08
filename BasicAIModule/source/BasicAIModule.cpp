#include <BWAPI.h>
#include <BWTA.h>
#include <Strategizer.h>
#include <BasicAIModule.h>
#include <EnhancedChokepoint.h>

vector<EnhancedChokepoint> ecPoints;

void BasicAIModule::onStart()
{

	BWTA::BaseLocation * baseLoc;
	BWTA::Region * region;
	set<BWTA::Chokepoint *> startLocChokepoints;
	set<BWTA::Chokepoint *>::iterator cpi;

	/* set up BWTA */
	BWTA::readMap();
	BWTA::analyze();
	//    BWSAL::resetLog();

	Broodwar->sendText("Hello, CS638!");
	Strategizer::instance().onMatchStart();

	m_enhancedUI = new EnhancedUI();

	/* get enhanced chokepoints for start location */
	baseLoc = BWTA::getStartLocation(Broodwar->self());
	region = baseLoc->getRegion();
	startLocChokepoints = region->getChokepoints();
	for (cpi = startLocChokepoints.begin(); cpi != startLocChokepoints.end(); ++cpi) {
		EnhancedChokepoint ecPoint(*cpi);
		ecPoints.push_back(ecPoint);
	}
}

void BasicAIModule::onEnd(bool isWinner)
{
}

void BasicAIModule::onFrame()
{
	BWTA::BaseLocation * baseLoc;
	BWTA::Region * region;

	baseLoc = BWTA::getStartLocation(Broodwar->self());
	region = baseLoc->getRegion();

	m_enhancedUI->update();

	/* additional UI */
	/* draw for enhanced chokepoints */
	for (int x = 0; x < (int) ecPoints.size(); ++x) {
		ecPoints[x].drawTilePositions();
		ecPoints[x].drawBoundingBox();
	}
	/* for show */
	m_enhancedUI->drawRegionBoundingBox(region);

	/* update the Strategizer */
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


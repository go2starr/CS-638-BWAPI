#pragma once
/* 
 * BasicAIModule.h
 */
#include "Strategizer.h"

#include <BWAPI.h>
#include <BWSAL.h>
#include <BWTA.h>

#include <EnhancedUI.h>
#include <TacticalBuildingPlacer.h>

#include <iterator>
#include <vector>
#include <string>
#include <set>


class BasicAIModule : public BWAPI::AIModule
{
private:
	EnhancedUI* enhancedUI;

public:
	virtual void onStart();
	virtual void onEnd(bool isWinner);
	virtual void onFrame();

	virtual void onUnitDiscover (BWAPI::Unit* unit);
	virtual void onUnitEvade    (BWAPI::Unit* unit);
	virtual void onUnitMorph    (BWAPI::Unit* unit);
	virtual void onUnitRenegade (BWAPI::Unit* unit);
	virtual void onUnitDestroy  (BWAPI::Unit* unit);

	virtual void onSendText(std::string text);

	void showPlayers();
	void showForces();
};

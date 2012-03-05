#pragma once
#include <BWAPI.h>
#include <BWTA.h>
#include <Strategizer.h>

using namespace std;
using namespace BWAPI;

class BasicAIModule : public BWAPI::AIModule
{

public:
  virtual void onStart();
  virtual void onEnd(bool isWinner);
  virtual void onFrame();
  virtual void onUnitDiscover(BWAPI::Unit* unit);
  virtual void onUnitEvade(BWAPI::Unit* unit);
  virtual void onUnitMorph(BWAPI::Unit* unit);
  virtual void onUnitRenegade(BWAPI::Unit* unit);
  virtual void onUnitDestroy(BWAPI::Unit* unit);
  virtual void onSendText(std::string text);
  void showPlayers();
  void showForces();
};

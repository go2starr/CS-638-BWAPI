#pragma once
#include <BWAPI.h>
#include <BWTA.h>

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
  ~BasicAIModule(); //not part of BWAPI::AIModule
  void showStats(); //not part of BWAPI::AIModule
  void showPlayers();
  void showForces();
};

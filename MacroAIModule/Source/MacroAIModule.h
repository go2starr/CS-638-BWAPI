#pragma once
#include <BWAPI.h>
#include <BWTA.h>
#include <Arbitrator.h>

class MacroAIModule : public BWAPI::AIModule
{
  public:
    MacroAIModule();
    ~MacroAIModule();
    virtual void onStart();
    virtual void onEnd(bool isWinner);
    virtual void onFrame();
    virtual void onSendText(std::string text);
    virtual void onUnitDiscover(BWAPI::Unit* unit);
    virtual void onUnitEvade(BWAPI::Unit* unit);
    virtual void onUnitDestroy(BWAPI::Unit* unit);
    virtual void onUnitMorph(BWAPI::Unit* unit);
    virtual void onUnitRenegade(BWAPI::Unit* unit);
  private:
    Arbitrator::Arbitrator<BWAPI::Unit*,double> arbitrator;
};
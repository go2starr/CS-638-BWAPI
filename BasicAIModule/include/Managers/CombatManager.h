/*
 *  CombatManager  -  Used to defend our base, and (eventually) attack other bases
 */
#pragma once
#include "Manager.h"
#include "Squad.h"

#include <BWAPI.h>

#include <vector>
#include <string>


class CombatManager : public Manager
{
private:
    AgentSet unassignedAgents;
    AgentSet assignedAgents;
    std::vector<Squad> squads;
    BWAPI::Position enemyBase;

public: 
    virtual void onMatchStart();
	virtual void update();
    
    virtual const std::string getName() const { return "CombatMgr"; }
};

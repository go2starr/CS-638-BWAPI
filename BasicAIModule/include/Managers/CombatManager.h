#pragma once
/*
 * CombatManager.h
 *  
 * Used to defend our base and attack other bases
 */
#include "Manager.h"
#include "Common.h"

#include <BWAPI.h>


class CombatManager : public Manager
{
private:
    AgentSet unassignedAgents;
    AgentSet assignedAgents;
    SquadVector squads;
    BWAPI::Position enemyBase;
    UnitSet enemyUnits;
    UnitSet enemyActors;
    UnitSet enemyBuildings;

public: 
    void discoverEnemyUnit(BWAPI::Unit* unit);
    virtual void onMatchStart();
    virtual void onMatchEnd(bool isWinner);
	virtual void update();
    virtual void draw();

    virtual const std::string& getName() const 
    { 
        static const std::string name("CombatMgr");
        return name;
    }

private:
	void addNewAgents();
    int numLivingAgents() const;

    BWAPI::Unit* findEnemyUnit(const BWAPI::UnitType& type);
    BWAPI::Unit* findNearestEnemyBuilding(const BWAPI::TilePosition& pos);
};

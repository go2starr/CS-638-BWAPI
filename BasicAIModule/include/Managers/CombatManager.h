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
	AgentSet bunkerAgents;
	// manage squad types separately
    SquadVector attackSquads;
	SquadVector defendSquads;
	SquadVector bunkerSquads;
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

	int numSquads() const;
	int numEnemyUnits() const;
	int numEnemyActors() const;
	int numEnemyBuildings() const;

    virtual const std::string& getName() const 
    { 
        static const std::string name("CombatMgr");
        return name;
    }

private:
	void addNewAgents();
    int numLivingAgents() const;
};

inline int CombatManager::numSquads() const 
{ 
	return attackSquads.size() +  defendSquads.size() + bunkerSquads.size();
}
inline int CombatManager::numEnemyUnits()     const { return enemyUnits.size(); }
inline int CombatManager::numEnemyActors()    const { return enemyActors.size(); }
inline int CombatManager::numEnemyBuildings() const { return enemyBuildings.size(); }

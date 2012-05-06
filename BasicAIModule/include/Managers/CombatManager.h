#pragma once
/*
 * CombatManager.h
 *  
 * Used to defend our base and attack other bases
 */
#include "Manager.h"
#include "Common.h"

#include <map>

#include <BWAPI.h>

using std::map;


class CombatManager : public Manager
{
private:
	// Map from Agents to Squads
	map<Agent*, Squad*> agentSquadMap;

	// Squad types
    SquadVector attackSquads;
	SquadVector defendSquads;

	// How large to make squads
	static const int AttackSquadSize = 15;
	static const int DefendSquadSize = 10;

    BWAPI::Position enemyBase;
    UnitSet enemyUnits;
    UnitSet enemyActors;
    UnitSet enemyBuildings;

public: 
    void discoverEnemyUnit(BWAPI::Unit* unit);
    virtual void onMatchStart();
    virtual void onMatchEnd(bool isWinner);

	virtual void update();
	virtual void updateSquadLeaders();

    virtual void draw();

	int numSquads() const;
	int numEnemyUnits() const;
	int numEnemyActors() const;
	int numEnemyBuildings() const;

	// Desired number of attack/defend squads
	unsigned getAttackSquadTargetSize() const;
	unsigned getDefendSquadTargetSize() const;

    virtual const std::string& getName() const 
    { 
        static const std::string name("CombatMgr");
        return name;
    }

private:
	void addNewAgents();
    int numLivingAgents() const;

	// Remove squads without agents
	void cleanUpSquads();
};

inline int CombatManager::numSquads() const 
{ 
	return attackSquads.size() +  defendSquads.size();
}
inline int CombatManager::numEnemyUnits()     const { return enemyUnits.size(); }
inline int CombatManager::numEnemyActors()    const { return enemyActors.size(); }
inline int CombatManager::numEnemyBuildings() const { return enemyBuildings.size(); }

#pragma once
/* 
 * Squad.h
 */
#include "Agent.h"
#include "Common.h"


/** TODO: use BWAPI::Position instead */
typedef std::pair<int, int> pt;

/** Squad Composition structure */
typedef struct 
{
    BWAPI::UnitType type;
    int currentNumber;
    int expectedNumber;
} SquadComposition;
typedef std::vector<SquadComposition>    SquadCompVector;
typedef SquadCompVector::iterator        SquadCompVectorIter;
typedef SquadCompVector::const_iterator  SquadCompVectorConstIter;

/** Squad Type enumeration */
enum SquadType {
    attack = 0, 
    defend, 
    explore, 
    bunker
};
extern const char* SquadTypeStrings[];


/************************************************************************/
/* Squad class
/************************************************************************/
class Squad
{
private:
    int id;

    SquadType type;

    bool active;

    Agent* leader;
    AgentSet agents;

public:
    Squad();
    Squad(const Squad& other);

    ~Squad();

    /* Assignment operator */
    Squad& operator=(const Squad& rhs);

    /* Update this squad, called every frame */
    void update();

    /* Draw any squad related on-screen info */
	void draw();

    /* Add the specified agent to this squad */
    void addAgent(Agent* agent);

    /* Remove the specified agent from this squad */
    void removeAgent(Agent* agent);

    /* Is the specified agent already a part of this squad? */
    bool isAssigned(Agent* agent);

    /* Set the leader of this squad, adding the specified agent if needed */
    void setLeader(Agent* agent);

    /* Gets a pointer to the current leader of this squad */
    Agent* getLeader();

    /* Gets the set of agents that are part of this squad */
    AgentSet& getAgents();

	/* Get the center of this squad */
	pt getCenter();

	/* Get the radius from center needed to cover units */
	int getRadius();
};

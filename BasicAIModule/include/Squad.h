#pragma once
/* 
 * Squad.h
 */
#include "Agent.h"
#include "Common.h"


/** TODO: use BWAPI::Position instead */
typedef std::pair<int, int> pt;

/** Squad Composition structure */
typedef struct squad_composition
{
    BWAPI::UnitType type;
    int currentNumber;
    int expectedNumber;

    squad_composition(const BWAPI::UnitType& type, 
                      const int expectedNumber)
        : type(type)
        , currentNumber(0)
        , expectedNumber(expectedNumber)
    { }
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
    static int nextAvailableId;

    std::string name;

    int  id;
    int  priority;
    bool active;    // only full squads should be active

    Agent*          leader;
    AgentSet        agents;
    
    SquadType       type;
    SquadCompVector composition;

public:
    Squad(const std::string& name, 
          const SquadType& type=attack, 
          const int priority=50);

    /* Update this squad, called every frame */
    void update();

    /* Draw any squad related on-screen info */
	void draw();

    /* Add the specified requirement in terms a desired number of a particular unit type */
    void addRequirement(const int numDesired, const BWAPI::UnitType& unitType);

    /* Add the specified agent to this squad */
    void addAgent(Agent* agent);

    /* Remove the specified agent from this squad */
    void removeAgent(Agent* agent);

    /* Is the specified agent already a part of this squad? */
    bool isAssigned(Agent* agent) const;

    /* Is this squad currently active? */
    bool isActive() const;

    /* Set the leader of this squad, adding the specified agent if needed */
    void setLeader(Agent* agent);

    /* Gets a pointer to the current leader of this squad */
    Agent* getLeader();

    /* Gets the set of agents that are part of this squad */
    AgentSet& getAgents();

    /* Gets the number of agents that are a part of this squad */
    int getSize() const;

	/* Get the center of this squad */
    BWAPI::Position getCenter();

	/* Get the radius from center needed to cover units */
	int getRadius();

    /* Get the name of this squad */
    const std::string& getName() const;

    /* Get the id for this squad */
    const int getId() const;

    /* Get the type for this squad */
    const SquadType& getType() const;

    /* Get the squad composition vector to look at */
    const SquadCompVector& getComposition() const;

    /* Get the number of living members in this squad */
    int numAlive() const;

private:
    Squad(const Squad& other); // Non-copyable
    void gatherTogether();
};


inline void Squad::removeAgent(Agent* agent) { agents.erase(agent); }
inline bool Squad::isAssigned(Agent* agent) const { return (agents.find(agent) != agents.end()); }
inline bool                   Squad::isActive() const { return active; }
inline Agent*                 Squad::getLeader()      { return leader; }
inline AgentSet&              Squad::getAgents()      { return agents; }
inline int                    Squad::getSize()  const { return agents.size(); }
inline const std::string&     Squad::getName()  const { return name; }
inline const int              Squad::getId()    const { return id; }
inline const SquadType&       Squad::getType()  const { return type; }
inline const SquadCompVector& Squad::getComposition() const { return composition; }
inline int Squad::numAlive() const 
{
    int num = 0;
    AgentSetConstIter it  = agents.begin();
    AgentSetConstIter end = agents.end();
    for(; it != end; ++it)
    {
        Agent *agent = *it;
        if( agent->getUnit().getHitPoints() > 0 )
            ++num;
    }
    return num;
}

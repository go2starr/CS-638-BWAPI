/*
 * Squad.cpp
 */
#include "Squad.h"
#include "Agent.h"
#include "Common.h"

#include <algorithm>
#include <cassert>

using namespace BWAPI;

using std::string;
using std::pair;
using std::min;
using std::max;



const char* SquadTypeStrings[] = { "attack", "defend", "explore", "bunker" };


/************************************************************************/
/* Squad class
/************************************************************************/
int Squad::nextAvailableId = 1;

Squad::Squad(const string& name, 
             const SquadType& type, 
             const int priority)
    : name(name)
    , id(nextAvailableId++)
    , priority(priority)
    , active(true)
    , leader(NULL)
    , agents()
    , type(type)
    , composition()
{ }

void Squad::update()
{
    assert(leader != NULL);
	// Find a new leader if current one dies
	if (leader->getUnit().getHitPoints() <= 0)
	{
		removeAgent(leader);
		for (AgentSetIter it = agents.begin(); it != agents.end(); it++)
		{
			leader = *it;
			break;
		}
	}

    // Do what the leader is doing
    AgentSetIter it  = agents.begin();
    AgentSetIter end = agents.end();
    for(; it != end; ++it)
    {
        Agent *agent = *it;
        if( agent != leader ) 
        {
            // agent->setState(leader->getState());
			agent->setState(AttackState);
			agent->setPositionTarget(leader->getPositionTarget());
            agent->setUnitTarget(leader->getUnitTarget());
            agent->setUnitTypeTarget(leader->getUnitTypeTarget());
        }
    }
}

void Squad::draw()
{
	pt center = getCenter();
	int radius = getRadius();

	int leaderX = leader->getUnit().getPosition().x();
	int leaderY = leader->getUnit().getPosition().y();

	// Draw lines from Agents to their leader
	for (AgentSetIter it = agents.begin(); it != agents.end(); it++)
	{
		int agentX = (*it)->getUnit().getPosition().x();
		int agentY = (*it)->getUnit().getPosition().y();
		Broodwar->drawLineMap(agentX, agentY, leaderX, leaderY, Colors::Teal);
	}
}

void Squad::addRequirement(const int numDesired, const UnitType& unitType)
{
    composition.push_back(SquadComposition(numDesired, unitType));
}

void Squad::addAgent(Agent* agent)
{
    agents.insert(agent);
}

inline void Squad::removeAgent(Agent* agent)
{
    agents.erase(agent);
}

inline bool Squad::isAssigned(Agent* agent) const
{ 
    return (agents.find(agent) != agents.end());
}

inline bool Squad::isActive() const
{
    return active;
}

void Squad::setLeader(Agent* agent)
{
    if( leader == agent ) 
        return;
    if( !isAssigned(agent) )
        addAgent(agent);
    leader = agent;
}

Agent* Squad::getLeader() 
{
    return leader;
}

AgentSet& Squad::getAgents()
{
    return agents;   
}

pt Squad::getCenter()
{
	int sumX, sumY;
	int x0, y0;
	sumX = sumY = 0;
	x0 = y0 = 0;

	for (AgentSetIter it = agents.begin(); it != agents.end(); it++)
	{
		int x = (*it)->getUnit().getPosition().x();
		int y = (*it)->getUnit().getPosition().y();
		sumX += x;
		sumY += y;
	}

	if (agents.size())
	{
		x0 = sumX / agents.size();
		y0 = sumY / agents.size();
	}
	return pt(x0, y0);
}

int Squad::getRadius()
{
	pt center = getCenter();
	Position centerPosition(center.first, center.second);
	int r = 0;
	for (AgentSetIter it = agents.begin(); it != agents.end(); it++)
	{
		r = (int)max(r, (*it)->getUnit().getDistance(centerPosition));
	}
	return r;
}

inline const string& Squad::getName() const
{
    return name;
}

inline const int Squad::getId() const
{
    return id;
}

inline const SquadType& Squad::getType() const
{
    return type;
}

inline const SquadCompVector& Squad::getComposition() const
{
    return composition;
}

/*
 * Squad.cpp
 */
#include "Squad.h"
#include "Agent.h"
#include "Common.h"

#include <algorithm>
#include <cassert>

using namespace BWAPI;
using std::min;
using std::max;
using std::pair;

Squad::Squad()
    : agents()
    , leader(NULL)
{ }

Squad::Squad(const Squad& other)
    : agents(other.agents)
    , leader(other.leader)
{ }

Squad::~Squad()
{
    agents.clear();
    leader = NULL;
}

Squad& Squad::operator=(const Squad& rhs)
{
    if( this != &rhs )
    {
        agents.clear();
        agents = rhs.agents;
        leader = rhs.leader;
    }
    return *this;
}

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
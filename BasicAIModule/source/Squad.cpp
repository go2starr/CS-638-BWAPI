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
	// Draw a box enclosing my squad
	int minX, maxX, minY, maxY;
	minX = minY = 999999;
	maxX = maxY = -1;
	int sumX, sumY;
	sumX = sumY = 0;
	for (AgentSetIter it = agents.begin(); it != agents.end(); it++)
	{
		int x = (*it)->getUnit().getPosition().x();
		int y = (*it)->getUnit().getPosition().y();
		sumX += x;
		sumY += y;
		minX = min(x, minX);
		maxX = max(x, maxX);
		minY = min(y, minY);
		maxY = max(y, maxY);
	}
	int x0, y0;
	if (agents.size())
	{
		x0 = sumX / agents.size();
		y0 = sumY / agents.size();
		Broodwar->drawEllipseMap(x0, y0, maxX-minX, maxY-minY, Colors::Cyan);
	}
}



/*
 * Squad.cpp
 */
#include "Squad.h"
#include "Agent.h"

#include <cassert>


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
            agent->setState(leader->getState());
            agent->setPositionTarget(leader->getPositionTarget());
            agent->setUnitTarget(leader->getUnitTarget());
            agent->setUnitTypeTarget(leader->getUnitTypeTarget());
        }
    }
}



#pragma once
/* 
 * Squad.h
 */
#include "Agent.h"
#include "Common.h"


class Squad
{
private:
    AgentSet agents;
    Agent* leader;

public:
    Squad();
    Squad(const Squad& other);
    ~Squad();

    Squad& operator=(const Squad& rhs);

    void update();

    inline void addAgent(Agent* agent)    { agents.insert(agent); }
    inline void removeAgent(Agent* agent) { agents.erase(agent); }

    inline bool isAssigned(Agent* agent)  
    { 
        if( agents.find(agent) != agents.end() )
            return true;
        else 
            return false;
    }

    inline AgentSet& getAgents() { return agents; }
    inline Agent* getLeader()    { return leader; }

    inline void setLeader(Agent* agent)
    {
        if( leader == agent ) 
            return;
        if( !isAssigned(agent) )
            addAgent(agent);
        leader = agent;
    }
};

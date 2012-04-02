#pragma once
/*
 * Common.h
 * 
 * Common typedefs and includes
 */
#include <sstream>
#include <string>
#include <vector>
#include <stack>
#include <queue>
#include <set>
#include <map>

class Agent;
class Squad;
class Task;
class Manager;

namespace BWAPI { class Unit; }
namespace BWAPI { class UnitType; }
namespace BWAPI { class Position; }
namespace BWAPI { class TilePosition; }

using std::vector;
using std::queue;
using std::stack;
using std::map;

/* Commonly used typedefs */

typedef std::set<Agent*>            AgentSet;
typedef AgentSet::iterator          AgentSetIter;
typedef AgentSet::const_iterator    AgentSetConstIter;

typedef std::vector<Agent*>         AgentVector;
typedef AgentVector::iterator       AgentVectorIter;
typedef AgentVector::const_iterator AgentVectorConstIter;

typedef std::vector<Squad*>         SquadVector;
typedef SquadVector::iterator       SquadVectorIter;
typedef SquadVector::const_iterator SquadVectorConstIter;

typedef std::set<BWAPI::Unit*>      UnitSet;
typedef UnitSet::iterator           UnitSetIter;
typedef UnitSet::const_iterator     UnitSetConstIter;

typedef std::map<BWAPI::Unit*, Agent*>  UnitAgentMap;
typedef UnitAgentMap::iterator          UnitAgentMapIter;
typedef UnitAgentMap::const_iterator    UnitAgentMapConstIter;

typedef std::map<Agent*, Manager*>      AgentManagerMap;
typedef AgentManagerMap::iterator       AgentManagerMapIter;
typedef AgentManagerMap::const_iterator AgentManagerMapConstIter;

typedef std::priority_queue<Task*>  TaskQueue;


struct DeleteObjectFunctor 
{
    template<class T>
    void operator()(T* ptr) const 
    {
        delete ptr;
    }
};

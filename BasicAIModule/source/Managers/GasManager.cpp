/*
 * GasManager.cpp
 *  
 * This manager sends it's workers to build refineries and gather gas 
 */
#include "GasManager.h"
#include "Common.h"
#include "Agent.h"

#include <BWAPI.h>

using namespace BWAPI;


GasManager::GasManager()
    : refineryCount(0)
    , refineryConstructingCount(0)
    , workersConstructing(0)
    , newRefineries(0)
{ }

void GasManager::update()
{
    // TODO: the refineries could be persistently stores by the GasMgr
    // so we can see who is gathering from where, and whether its exhausted
	// TODO: worry about gas steal?

	Broodwar->drawTextScreen(2, 30, "\x10 GM : (SCV=%d)", numAgents(UnitTypes::Terran_SCV));

    AgentSet refineries(getAgentsOfType(UnitTypes::Terran_Refinery));

    // Get a count on new refineries
    newRefineries = static_cast<int>(refineries.size()) - refineryCount;
	refineryCount = static_cast<int>(refineries.size());

	// Keep the workers constructing count up to date
    // TODO: remove this? is this responsibility being delegated to the BuildMgr?
    //*
	if( newRefineries > 0 ) 
    {
//		refineryConstructingCount -= newRefineries;
		workersConstructing -= newRefineries;
	}
    //*/

    AgentSet workers(getAgentsOfType(UnitTypes::Terran_SCV));

    // Set a worker to build a refinery if we need to (for now just grab the first one)
    // TODO: remove this? is this responsibility being delegated to the BuildMgr?
    //*
    if( refineries.empty() && !workers.empty() && workersConstructing == 0 )
    {
        //Broodwar->sendText("Gas manager, search for agent out of %d\n", (int)workers.size());
        Agent* worker = *(workers.begin());
        worker->setState(BuildState);
        worker->setUnitTypeTarget(UnitTypes::Terran_Refinery);
        // TODO - worker->setPositionTarget() to where?  
        // how does this worker know where to build?
        ++workersConstructing;
    }
    //*/

    // Get our workers gathering from our refineries
	for(AgentSetIter worker = workers.begin(); worker != workers.end(); ++worker)
	{
		Unit& unit = (*worker)->getUnit();
		if( !unit.isGatheringGas() && !unit.isConstructing() ) 
        {
			for(AgentSetIter refinery = refineries.begin(); refinery != refineries.end(); refinery++) 
            {
                Unit& refineryUnit = (*refinery)->getUnit();
                (*worker)->setState(GatherState);
				(*worker)->setUnitTarget(&refineryUnit);
                (*worker)->setUnitTypeTarget(UnitTypes::Terran_Refinery);
                (*worker)->setPositionTarget(refineryUnit.getPosition());
			}
		}
	}

	// Update all agents, very important!
	Manager::update();
}

int GasManager::getGasRate() const
{
    // TODO
	return 0;
}

int GasManager::getNumWorkersGathering() const
{
    int count = 0;
    AgentSetConstIter it  = agents.begin();
    AgentSetConstIter end = agents.end();
    for(; it != end;)
    {
        Agent *agent = *it;
        if( agent->getUnit().isGatheringGas() )
        {
            ++count;
        }
    }
    return count;
}

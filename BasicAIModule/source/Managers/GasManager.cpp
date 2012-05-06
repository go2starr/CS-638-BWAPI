/*
 * GasManager.cpp
 *  
 * This manager sends it's workers to build refineries and gather gas 
 */
#include "GasManager.h"
#include "Common.h"
#include "Agent.h"
#include "BuildManager.h"
#include "Strategizer.h"

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
    AgentSet refineries(getAgentsOfType(UnitTypes::Terran_Refinery));

	if( refineries.empty() && agents.size() )
    {
		// TODO: HACK.  Fix me
		static bool planned = false;
		if (!planned) {
			planned = true;
			Strategizer::instance().buildManager.build(UnitTypes::Terran_Refinery, 1, 1);
		}
    }

    // Get our workers gathering from our refineries
	AgentSet workers = agents;
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

void GasManager::draw()
{
	Manager::draw();
}

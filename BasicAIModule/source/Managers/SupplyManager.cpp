#include "SupplyManager.h"
#include <BWAPI.h>

using namespace BWAPI;
using namespace std;


SupplyManager::SupplyManager()
    : supplyDepotCount(0)
    , supplyDepotConstructingCount(0)
    , workersConstructing(0)
    , newSupplyDepots(0)
{ }

void SupplyManager::update()
{
	Broodwar->drawTextScreen(2, 10, "\x1E SM : (SCV=%d)", numAgents(UnitTypes::Terran_SCV));

	// how many supply depots do we own
	// there are no supply depot agents currently :(
	//  int count = numAgents(UnitTypes::Terran_Supply_Depot);

	//	newSupplyDepots = count - supplyDepotCount;
	//	supplyDepotCount = count;

	// workers done building supply depots
	//if (newSupplyDepots) {
	//  supplyDepotConstructingCount -= newSupplyDepots;
	//  workersConstructing -= newSupplyDepots;
	//}

	// Build supply depots if we are running low aren't already building them
	if (Broodwar->self()->supplyTotal() - Broodwar->self()->supplyUsed() < 8)
	{
		for (AgentSetIter it = agents.begin(); it != agents.end(); it++)
		{
			Agent *agent = *it;
			if(agent->getState() != BuildState) 
            {
				agent->setState(BuildState);
				agent->setUnitTypeTarget(UnitTypes::Terran_Supply_Depot);
				// not exactly true, we don't know if it's really being constructed
				// supplyDepotConstructingCount++;
				// workersConstructing++;
				Broodwar->sendText("Supply manager, set agent to build");
				break;
			}
		}
	}

	/* Base class updates Agents */
	Manager::update();
}

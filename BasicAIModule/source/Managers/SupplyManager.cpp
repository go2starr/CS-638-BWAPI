#include "SupplyManager.h"
#include <BWAPI.h>

using namespace BWAPI;
using namespace std;

SupplyManager::SupplyManager()
{
	suppplyDepotCount = 0;
	suppplyDepotConstructingCount = 0;
	worksersConstructing = 0;
	newSuppplyDepots = 0;
}

void SupplyManager::update()
{
	Broodwar->drawTextScreen(2, 10, "\x1E SM : (SCV=%d)", numAgents(UnitTypes::Terran_SCV));

	// how many supply depots do we own
	// there are no supply depot agents currently :(
	//int count = numAgents(UnitTypes::Terran_Supply_Depot);

	//	newSuppplyDepots = count - suppplyDepotCount;
	//	suppplyDepotCount = count;

	// workers done building supply depots
	if (newSuppplyDepots) {
		//suppplyDepotConstructingCount -= newSuppplyDepots;
		//worksersConstructing -= newSuppplyDepots;
	}

	// Build supply depots if we are running low aren't already building them

	if (Broodwar->self()->supplyTotal() - Broodwar->self()->supplyUsed() < 8)
	{

		//			Broodwar->sendText("Supply manager, search for agent out of %d\n", (int)agents.size());

		for (set<Agent*>::iterator it = agents.begin(); it != agents.end(); it++)
		{
			Agent *agent = *it;
			if(agent->getState() != BuildState) {

				agent->setState(BuildState);
				agent->setUnitTypeTarget(BWAPI::UnitTypes::Terran_Supply_Depot);
				// not exactly true, we don't know if it's really being constructed
				//suppplyDepotConstructingCount++;
				//				worksersConstructing++;
				Broodwar->sendText("Supply manager, set agent to build");
				break;

			}
		}
	}


	/* Base class updates Agents */
	Manager::update();
}
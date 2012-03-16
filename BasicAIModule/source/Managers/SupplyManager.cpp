#include "SupplyManager.h"

#include <Strategizer.h>
#include <Managers/BuildManager.h>

#include <BWAPI.h>

using namespace BWAPI;
using namespace std;


SupplyManager::SupplyManager()
    : depotCount(0)
	, plannedDepotCount(0)
{ }

void SupplyManager::update()
{
	Broodwar->drawTextScreen(2, 10, "\x1E SM : %d planned", plannedSupply()/2);
	
	/* Build supply if running low */
	int currentSupply = BWAPI::Broodwar->self()->supplyUsed();
	if (plannedSupply() - currentSupply < 6)
	{
		plannedDepotCount++;
		Strategizer::instance().buildManager.build(BWAPI::UnitTypes::Terran_Supply_Depot, true);
	}

	/* Base class updates Agents */
	Manager::update();
}

int SupplyManager::plannedSupply()
{
	int plannedSupply = plannedDepotCount * BWAPI::UnitTypes::Terran_Supply_Depot.supplyProvided();
	int totalSupply = BWAPI::Broodwar->self()->supplyTotal();
	return plannedSupply + totalSupply;
}

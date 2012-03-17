/*
 *  SupplyManager.cpp
 *  
 *  Used to ensure SupplyDepot count keeps supply below capacity
 */
#include "SupplyManager.h"
#include "BuildManager.h"
#include "Strategizer.h"

#include <BWAPI.h>

using namespace BWAPI;


SupplyManager::SupplyManager()
    : depotCount(0)
	, plannedDepotCount(0)
{ }

void SupplyManager::update()
{
	Broodwar->drawTextScreen(2, 10, "\x1E SM : %d planned", plannedSupply()/2);
	
	/* Build supply if running low */
	int currentSupply = Broodwar->self()->supplyUsed();
	if (plannedSupply() - currentSupply < 6)
	{
		plannedDepotCount++;
		Strategizer::instance().buildManager.build(UnitTypes::Terran_Supply_Depot, true);
	}

	/* Base class updates Agents */
	Manager::update();
}

int SupplyManager::plannedSupply() const
{
    // Note: supplyProvided() returns double the onscreen supply count
	const int plannedSupply = plannedDepotCount * UnitTypes::Terran_Supply_Depot.supplyProvided();
	const int totalSupply   = Broodwar->self()->supplyTotal();
	return plannedSupply + totalSupply;
}

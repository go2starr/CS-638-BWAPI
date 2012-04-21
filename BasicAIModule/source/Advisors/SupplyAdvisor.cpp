/*
 *  SupplyAdvisor.cpp
 *  
 *  Used to ensure SupplyDepot count keeps supply below capacity
 */
#include "SupplyAdvisor.h"
#include "Strategizer.h"
#include "BuildManager.h"

#include <BWAPI.h>

using namespace BWAPI;


int SupplyAdvisor::depotCount        = 0;
int SupplyAdvisor::plannedDepotCount = 0;


void SupplyAdvisor::update()
{
	const Player *self = Broodwar->self();
	const int MIN_SUPPLY_DIFF = 12;

	// Build supply if running low
	const int currentSupply = self->supplyUsed();
	const int supplyDiff    = plannedSupply() - currentSupply;
	if( supplyDiff < MIN_SUPPLY_DIFF )
	{
		Broodwar->sendText(
			"SM: Nearing capacity (%d/%d), Supply Depot ordered",
			currentSupply, plannedSupply() );

		++plannedDepotCount;

		Strategizer::instance().buildManager.build(UnitTypes::Terran_Supply_Depot, true);
	}

	// Detect Supply Depot completion
	const int completedDepots = self->completedUnitCount(UnitTypes::Terran_Supply_Depot);
	if( completedDepots > depotCount )
	{
		++depotCount;
		--plannedDepotCount;
	}
}

int SupplyAdvisor::plannedSupply()
{
	// Note: supplyProvided() returns double the onscreen supply count
	const int plannedSupply = plannedDepotCount * UnitTypes::Terran_Supply_Depot.supplyProvided();
	const int totalSupply   = Broodwar->self()->supplyTotal();
	return plannedSupply + totalSupply;
}

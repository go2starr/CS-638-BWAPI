/*
 *  SupplyManager - Used to ensure SupplyDepot count keeps supply below capacity
 */
#pragma once
#include "Manager.h"

class SupplyManager : public Manager
{
public: 
	SupplyManager();
	virtual void update();

private:
    int supplyDepotCount;
	int supplyDepotConstructingCount;
	int workersConstructing;
	int newSupplyDepots;
};

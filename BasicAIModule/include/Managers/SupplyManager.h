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

    virtual const std::string getName() const { return "SupplyMgr"; }

private:
    int depotCount;
	int plannedDepotCount;

	/* plannedSupply - plannedSupply from planned depots + currentSupply*/
	int plannedSupply();
};

#pragma once
/*
 *  SupplyAdvisor.h
 *  
 *  Used to ensure SupplyDepot count keeps supply below capacity
 */


class SupplyAdvisor
{
private:
    static int depotCount;
    static int plannedDepotCount;

public: 
	static void update();
	static int plannedSupply();
};

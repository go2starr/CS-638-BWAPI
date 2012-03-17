#pragma once
/*
 *  SupplyManager.h
 *  
 *  Used to ensure SupplyDepot count keeps supply below capacity
 */
#include "Manager.h"


class SupplyManager : public Manager
{
private:
    int depotCount;
    int plannedDepotCount;

    /* plannedSupply - plannedSupply from planned depots + currentSupply*/
    int plannedSupply() const;

public: 
	SupplyManager();

	virtual void update();

    virtual const std::string& getName() const 
    { 
        static const std::string name("SupplyMgr");
        return name;
    }
};

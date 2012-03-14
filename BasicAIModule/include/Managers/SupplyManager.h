/*
 *  SupplyManager - Used to ensure SupplyDepot count keeps supply below capacity
 */
#pragma once
#include "Manager.h"

class SupplyManager : public Manager
{
public: 
	SupplyManager();
	void update();

	private:

	 int suppplyDepotCount;
	int suppplyDepotConstructingCount;
	int worksersConstructing;
	int newSuppplyDepots;
};

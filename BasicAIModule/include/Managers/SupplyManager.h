/*
 *  SupplyManager - Used to ensure SupplyDepot count keeps supply below capacity
 */
#pragma once
#include "Manager.h"

class SupplyManager : public Manager
{
public: 
	void update();
};

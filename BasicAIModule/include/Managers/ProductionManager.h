/*
 *  ProductionManager  -  Used to train new units
 */
#pragma once
#include "Manager.h"

class ProductionManager : public Manager
{
public:
	void update();

    virtual const std::string getName() const { return "ProductionMgr"; }
};

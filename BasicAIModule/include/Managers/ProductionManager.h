#pragma once
/*
 *  ProductionManager.h
 *  
 *  Used to train new units
 */
#include "Manager.h"


class ProductionManager : public Manager
{
public:
	void update();

    virtual const std::string& getName() const 
    {
        static const std::string name("ProductionMgr");
        return name; 
    }
};

#pragma once
/*
 * ConstructionManager.h
 * 
 * Used to produce new structures
 */
#include "Manager.h"

class ConstructionManager : public Manager
{
public: 
	void update();

    virtual const std::string& getName() const 
    {
        static const std::string name("ConstructionMgr");
        return name; 
    }
};

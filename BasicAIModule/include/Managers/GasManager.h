#pragma once
/*
 *  GasManager.h 
 *  
 *  This manager sends it's workers to build refineries and gather gas 
 */
#include "Manager.h"


class GasManager : public Manager
{
private:
    int refineryCount;
    int refineryConstructingCount;
    int workersConstructing;
    int newRefineries;

public:
    GasManager();

	virtual void update();
	
    int getGasRate() const;
    int getNumWorkersGathering() const;

    virtual const std::string& getName() const 
    {
        static const std::string name("GasMgr");
        return name; 
    }
};

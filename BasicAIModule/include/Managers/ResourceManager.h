#pragma once
/*
 *  ResourceManager.h  
 *  
 *  This manager sends it's workers to gather gas and minerals at maximum efficiency.
 */
#include "Manager.h"


class ResourceManager : public Manager
{
public:
	void update();

	int getMineralRate() const;
    int getNumWorkersGathering() const;

    virtual const std::string& getName() const 
    {
        static const std::string name("ResourceMgr");
        return name; 
    }
};

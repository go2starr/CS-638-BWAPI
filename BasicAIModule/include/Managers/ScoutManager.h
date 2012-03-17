/*
 *  ScoutManager - Used to find enemy base location and explore the map 
 */
#pragma once
#include "Manager.h"

class ScoutManager : public Manager
{
public: 
	void update();

    virtual const std::string getName() const { return "ScoutMgr"; }
};

/*
 *  ScoutManager - Used to find enemy base location and explore the map 
 */
#pragma once
#include "Manager.h"
#include "MapAdvisor.h"


class ScoutManager : public Manager
{
private:


public: 
	static const int CONTROLVALUEVALUE = 10;
	static const int DISTANCEVALUE = 1;
	static const int FRAMECOUNTVALUE = 100;

	void update();
	void draw();
	BWAPI::TilePosition getScoutTilePosition(BWAPI::TilePosition tilePosition);

	void scratch();

    virtual const std::string& getName() const 
    { 
        static const std::string name("ScoutMgr");
        return name;
    }
};

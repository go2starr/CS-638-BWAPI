/*
 *  ScoutManager - Used to find enemy base location and explore the map 
 */
#pragma once
#include "Manager.h"
#include "MapAdvisor.h"
#include <iostream>
#include <fstream>


struct ScoutMapBlock {
	double stratigicValue;
	double lastVisibleValue;
	double distanceValue;
	double Value;
	bool reachable;
	bool selected;
};

class ScoutManager : public Manager
{
private:


	void log(int x, int y);

public: 
	static const int CONTROLVALUEVALUE = 10;
	static const int DISTANCEVALUE = 100;
	static const int FRAMECOUNTVALUE = 100;
	static const int LOGFREQUENCY = 1000;

	void onMatchStart();
	void onMatchEnd();
	void update();
	void draw();

	std::ofstream scoutManagerLogFile;
	BWAPI::TilePosition getScoutTilePosition(BWAPI::TilePosition tilePosition, bool isFlyer);

	ScoutMapBlock scoutMapBlocks[MapAdvisor::blockXCount][MapAdvisor::blockYCount];	

    virtual const std::string& getName() const 
    { 
        static const std::string name("ScoutMgr");
        return name;
    }
};

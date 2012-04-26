#pragma once
/*
 *  MapAdvisorr.h
 *  
 *  Used to store data about the map
 *  Provies utility functions that inform other managers about the state of the map
 */

#include "Common.h"

#include <BWAPI.h>
#include <iostream>
#include <fstream>


// Information needed to complete a build request
struct Coordinate {
	int x;
	int y;
};

struct MapBlock {
	Coordinate upperLeftCoordinate;
	Coordinate mainCoordinate;

//	BWAPI::UnitType type;
//	BWAPI::Player controlPlayer;
	int controlLevel;		//The strength of the strongest presence in the block
	int stratigicValue;		//The stratigic value of the block
	int lastVisibileFrame;	//The last frame for which the main tile was visible
	int gasAvailable;		//The amount of gas available in the square
	int mineralsAvailable;	//The amount of minerals available in the square
	bool chokePoint;		//If the block contains a choke point
	bool startLocation;		//If the block contains a start location
	// TODO: track existance / ownership of gas extractor
	// TODO: Add base reference
} ;


class MapAdvisor
{
private:

	static const int CHOKEPOINTVALUE = 20;
	static const int STARTLOCATIONVALUE = 50;
	static const int GASVALUE = 1000;
	static const int MINERALSVALUE = 1000;
	static const int LOCATIONVALUE = 25;


//	static std::ofstream mapAdvisorLogFile;


    static void drawDebugText();

	static int getStratigicValue(MapBlock mapBlock);
	static int getControlLevel(MapBlock mapBlock);

	static int getLocationControl(int x, int y);


public: 

	
	// the block?Count should be a factor of 32.
	// Also, needs to match the array size for mapBlocks in MapAdvisor.h
	static const int blockXCount = 16;		// Number of blocks in the X direction
	static const int blockYCount = 16;		// Number of blocks in the Y direction

	static int blockXLength;	// Number of tiles in each block in the X direction
	static int blockYLength;	// Number of tiles in each block in the Y direction

	static MapBlock mapBlocks[blockXCount][blockYCount];

	static void init(int mapX, int mapY);
	static MapBlock tileToBlock(int x, int y);
	static void update();
	static void draw();
	static void onMatchEnd();
	static void mapLog();

};

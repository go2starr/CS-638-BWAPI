#pragma once
/*
 *  MapAdvisorr.h
 *  
 *  Used to store data about the map
 *  Provies utility functions that inform other managers about the state of the map
 */

#include "Common.h"

#include <BWAPI.h>


// Information needed to complete a build request
struct MapBlock {
	int upperLeftX;
	int upperLeftY;
	int lengthX;
	int lengthY;
	
//	BWAPI::UnitType type;
//	BWAPI::Player controlPlayer;
	int controlLevel;
	int controlValue;
	// TODO: Add base reference
} ;


class MapAdvisor
{
private:

    static void drawDebugText();

//	static const int blockXCount = 16;
//	static const int blockYCount = 16;

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

};

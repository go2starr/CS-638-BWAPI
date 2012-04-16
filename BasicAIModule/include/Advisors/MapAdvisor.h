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
typedef struct MapBlock {
	int upperLeftX;
	int upperLeftY;
	int lengthX;
	int lengthY;
	
//	BWAPI::UnitType type;
//	BWAPI::Player controlPlayer;
	int controlLevel;
	int controlValue;
	// TODO: Add base reference
} MapBlock;


class MapAdvisor
{
private:

    static void drawDebugText();

public: 
	static int blockXCount;		// Number of blocks in the X direction
	static int blockYCount;		// Number of blocks in the Y direction
	static int blockXLength;	// Number of tiles in each block in the X direction
	static int blockYLength;	// Number of tiles in each block in the Y direction

	static MapBlock mapBlocks[16][16];

	static void init(int mapX, int mapY);
	static MapBlock tileToBlock(int x, int y);
	static void update();
	static void draw();

};

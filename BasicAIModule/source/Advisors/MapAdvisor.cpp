/*
 *  MapAdvisor.cpp
 *  
 *  Used to store and return data about the map
 */
#include "MapAdvisor.h"
#include "Common.h"

#include <BWAPI.h>
#include <BWTA.h>

using namespace BWAPI;

using std::deque;
using std::map;


	// the block?Count should be a factor of 32.
	// Also, needs to match the array size for mapBlocks in MapAdvisor.h
//	int MapAdvisor::blockXCount = 16;
//	int MapAdvisor::blockYCount = 16;
	int MapAdvisor::blockXLength = -1;
	int MapAdvisor::blockYLength = -1;

MapBlock mapBlocks[MapAdvisor::blockXCount][MapAdvisor::blockYCount];


MapBlock tileToBlock(int x, int y) {
	// Returns the block which contains a bulld tile with coordinates x,y.
	int blockX = x / MapAdvisor::blockXLength;
	int blockY = y / MapAdvisor::blockYLength;

	return mapBlocks[blockX][blockY];
}


void MapAdvisor::init(int mapX, int mapY){

	// set the block lengths
	blockXLength = mapX / blockXCount;
	blockYLength = mapY / blockYCount;


	for(int i = 0 ; i < blockXCount ; i++)
		for (int j = 0 ; j < blockYCount ; j++)
		{
//			mapBlocks[i][j] = new MapBlock;
		}

}

void MapAdvisor::update(){}

void MapAdvisor::draw(){}


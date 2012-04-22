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

MapBlock MapAdvisor::mapBlocks[MapAdvisor::blockXCount][MapAdvisor::blockYCount];

// returns the block that the position belongs to
MapBlock MapAdvisor::tileToBlock(int x, int y) {
	// Returns the block which contains a bulld tile with coordinates x,y.
	int blockX = x / MapAdvisor::blockXLength;
	int blockY = y / MapAdvisor::blockYLength;

	return MapAdvisor::mapBlocks[blockX][blockY];
}

// Determines and returns the Control Value based on its location on the map
int MapAdvisor::getLocationControl(int x, int y)
{
	int controlValue = 0;

	controlValue += std::min(x, MapAdvisor::blockXCount - x - 1);
	controlValue += std::min(y, MapAdvisor::blockYCount - y - 1);
	controlValue *= MapAdvisor::LOCATIONVALUE;

	return controlValue;
}
// Determines and returns the current control value for the block
int MapAdvisor::getControlValue(MapBlock mapBlock){

	int controlValue = 0;

	// is there a choice point on the block?
	controlValue += mapBlock.chokePoint * MapAdvisor::CHOKEPOINTVALUE;

	// is there a starting location on the block?
	controlValue += mapBlock.startLocation * MapAdvisor::STARTLOCATIONVALUE;

	// are there any resources on the map
	controlValue += mapBlock.gasAvailable / MapAdvisor::GASVALUE;
	controlValue += mapBlock.mineralsAvailable / MapAdvisor::MINERALSVALUE;

	// Location value
	controlValue += MapAdvisor::getLocationControl(mapBlock.upperLeftCoordinate.x / MapAdvisor::blockXLength, mapBlock.upperLeftCoordinate.y / MapAdvisor::blockYLength);

	return controlValue;
}

// Determines and returns the level of control in the block
// Positive values for friendly control, negative values for opponent's control
int MapAdvisor::getControlLevel(MapBlock mapBlock){

	int controlLevel = 0;

	// net number of buildings
	//TODO

	// net units by supply values
	//TODO

	return controlLevel;

}

void MapAdvisor::init(int mapX, int mapY){

	// set the block lengths
	MapAdvisor::blockXLength = mapX / MapAdvisor::blockXCount;
	MapAdvisor::blockYLength = mapY / MapAdvisor::blockYCount;


	for(int i = 0 ; i < MapAdvisor::blockXCount ; i++)
		for (int j = 0 ; j < MapAdvisor::blockYCount ; j++)
		{
			MapBlock& mapBlock = mapBlocks[i][j];

			mapBlock.upperLeftCoordinate.x = i * MapAdvisor::blockXLength;
			mapBlock.upperLeftCoordinate.y = j * MapAdvisor::blockYLength;
			mapBlock.mainCoordinate.x = mapBlock.upperLeftCoordinate.x + (MapAdvisor::blockXLength / 2);
			mapBlock.mainCoordinate.y = mapBlock.upperLeftCoordinate.y + (MapAdvisor::blockYLength / 2);
			mapBlock.controlLevel = 0;
			mapBlock.controlValue = 0;
		}

		// Cycle through regions..
			// Cycle through choke points 
			// TODO
			
			// Cycle through bases
			//TODO		

			// Cycle through start points
			//TODO
}

void MapAdvisor::update()
{
	// Loop through blocks
	for(int i = 0 ; i < blockXCount ; i++)
		for (int j = 0 ; j < blockYCount ; j++)
		{
			MapBlock& mapBlock = mapBlocks[i][j];

			// See if it's visible
			if(Broodwar->isVisible(mapBlock.mainCoordinate.x, mapBlock.mainCoordinate.y))
			{
				// Update lastVisibleFrame
				mapBlock.lastVisibileFrame = Broodwar->getFrameCount();
				// If visible, update controlValue
				mapBlock.controlValue = MapAdvisor::getControlValue(mapBlock);
				// Update control Level (start with simple count)
				mapBlock.controlLevel = MapAdvisor::getControlLevel(mapBlock);

			}
		}

}

void MapAdvisor::draw(){}


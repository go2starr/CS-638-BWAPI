/*
 *  MapAdvisor.cpp
 *  
 *  Used to store and return data about the map
 */
#include "MapAdvisor.h"
#include "Common.h"

#include <BWAPI.h>
#include <BWTA.h>
#include <shlobj.h>

using namespace BWAPI;

using std::deque;
using std::map;


	int MapAdvisor::blockXLength = -1;
	int MapAdvisor::blockYLength = -1;
	std::ofstream mapAdvisorLogFile;

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

	int xRight = MapAdvisor::blockXCount -x -1;
	int yBottom = MapAdvisor::blockYCount -y -1;

//	controlValue += std::min(x, xRight);
//	controlValue += std::min(y, yBottom);
	if (x < xRight) 
		controlValue += x;
	else
		controlValue += xRight;

	if (y < yBottom)
		controlValue += y;
	else
		controlValue += yBottom;


	controlValue *= MapAdvisor::LOCATIONVALUE;

	return controlValue;
}
// Determines and returns the current control value for the block
int MapAdvisor::getStratigicValue(MapBlock mapBlock){

	int stratigicValue = 0;

	// is there a choice point on the block?
	stratigicValue += mapBlock.chokePoint * MapAdvisor::CHOKEPOINTVALUE;

	// is there a starting location on the block?
	stratigicValue += mapBlock.startLocation * MapAdvisor::STARTLOCATIONVALUE;

	// are there any resources on the map
	stratigicValue += mapBlock.gasAvailable / MapAdvisor::GASVALUE;
	stratigicValue += mapBlock.mineralsAvailable / MapAdvisor::MINERALSVALUE;

	// Location value
	stratigicValue += MapAdvisor::getLocationControl(mapBlock.upperLeftCoordinate.x / MapAdvisor::blockXLength, mapBlock.upperLeftCoordinate.y / MapAdvisor::blockYLength);

	return stratigicValue;
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
			mapBlock.stratigicValue = 0;
		}

		// Cycle through regions..
			// Cycle through choke points 
			// TODO
			
			// Cycle through bases
			//TODO		

			// Cycle through start points
			//TODO

	// Create Log file.
	mapAdvisorLogFile.open("c:\\MapAdvisor.log");
}

void MapAdvisor::onMatchEnd()
{
	mapAdvisorLogFile.close();
}

void MapAdvisor::update()
{
	// Loop through blocks
	for(int i = 0 ; i < blockXCount ; i++)
		for (int j = 0 ; j < blockYCount ; j++)
		{
			MapBlock& mapBlock = mapBlocks[i][j];

			// Update stratigicValue
			mapBlock.stratigicValue = MapAdvisor::getStratigicValue(mapBlock);

			// See if it's visible
			if(Broodwar->isVisible(mapBlock.mainCoordinate.x, mapBlock.mainCoordinate.y))
			{
				// Update lastVisibleFrame
				mapBlock.lastVisibileFrame = Broodwar->getFrameCount();
				// Update control Level (start with simple count)
				mapBlock.controlLevel = MapAdvisor::getControlLevel(mapBlock);

			}
		}

	if (Broodwar->getFrameCount() % 1000 == 0)
		MapAdvisor::mapLog();

}

void MapAdvisor::draw(){}

void MapAdvisor::mapLog()
{
	
	mapAdvisorLogFile << "\n\nFrame:	" ;
	mapAdvisorLogFile << Broodwar->getFrameCount();
	for (int j = 0 ; j < MapAdvisor::blockYCount ; j++)
	{
		mapAdvisorLogFile << "\nMapBlock       :";
		for (int i = 0 ; i < MapAdvisor::blockXCount ; i++)
			mapAdvisorLogFile << "	" << i << ", " << j;

		mapAdvisorLogFile << "\nControl Value  :";
		for (int i = 0 ; i < MapAdvisor::blockXCount ; i++)
			mapAdvisorLogFile << "	" << MapAdvisor::mapBlocks[i][j].controlLevel;

		mapAdvisorLogFile << "\nStratigic Value:";
		for (int i = 0 ; i < MapAdvisor::blockXCount ; i++)
			mapAdvisorLogFile << "	" << MapAdvisor::mapBlocks[i][j].stratigicValue;

		mapAdvisorLogFile << "\nVisible Frame  :";
		for (int i = 0 ; i < MapAdvisor::blockXCount ; i++)
			mapAdvisorLogFile << "	" << MapAdvisor::mapBlocks[i][j].lastVisibileFrame;
		
		mapAdvisorLogFile << "\nGas Available  :";
		for (int i = 0 ; i < MapAdvisor::blockXCount ; i++)
			mapAdvisorLogFile << "	" << MapAdvisor::mapBlocks[i][j].gasAvailable;

		mapAdvisorLogFile << "\nMineral Avail. :";
		for (int i = 0 ; i < MapAdvisor::blockXCount ; i++)
			mapAdvisorLogFile << "	" << MapAdvisor::mapBlocks[i][j].mineralsAvailable;

		mapAdvisorLogFile << "\nChoke Point    :";
		for (int i = 0 ; i < MapAdvisor::blockXCount ; i++)
			mapAdvisorLogFile << "	" << MapAdvisor::mapBlocks[i][j].chokePoint;
		
		mapAdvisorLogFile << "\nStart Location :";
		for (int i = 0 ; i < MapAdvisor::blockXCount ; i++)
			mapAdvisorLogFile << "	" << MapAdvisor::mapBlocks[i][j].startLocation;

		mapAdvisorLogFile << "\nUL Coordinates :";
		for (int i = 0 ; i < MapAdvisor::blockXCount ; i++)
			mapAdvisorLogFile << "	" << MapAdvisor::mapBlocks[i][j].upperLeftCoordinate.x << "," << MapAdvisor::mapBlocks[i][j].upperLeftCoordinate.y;
		
		mapAdvisorLogFile << "\nCtr Coordinates:";
		for (int i = 0 ; i < MapAdvisor::blockXCount ; i++)
			mapAdvisorLogFile << "	" << MapAdvisor::mapBlocks[i][j].mainCoordinate.x << "," << MapAdvisor::mapBlocks[i][j].mainCoordinate.y;
		mapAdvisorLogFile << "\n";
	}
	
}
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

using std::pair;
using std::set;


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

	// is it a base locaiton
	stratigicValue += mapBlock.baseLocation * MapAdvisor::BASELOCATIONVALUE;

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


		// Initialize Stratigic Values
		MapAdvisor::initStratigicValues();

	// Create Log file.
	mapAdvisorLogFile.open("c:\\MapAdvisor.log");
}

void MapAdvisor::initStratigicValues()
{
	int xBlock;
	int yBlock;

	// Update Starting Locations
	set<TilePosition>& startPositions = Broodwar->getStartLocations();
	set<TilePosition>::iterator pit  = startPositions.begin();
	set<TilePosition>::iterator pend = startPositions.end();
	for(; pit != pend; ++pit)
	{
		TilePosition pos = *pit;
		xBlock = pos.x() / MapAdvisor::blockXLength;
		yBlock = pos.y() / MapAdvisor::blockYLength;

		MapBlock& mapBlock = MapAdvisor::mapBlocks[xBlock][yBlock];
		mapBlock.startLocation = 1;
	}

	// Update Base Locations
	for(std::set<BWTA::BaseLocation*>::const_iterator i=BWTA::getBaseLocations().begin();i!=BWTA::getBaseLocations().end();i++)
	{
		TilePosition pos = (*i)->getTilePosition();
		xBlock = pos.x() / MapAdvisor::blockXLength;
		yBlock = pos.y() / MapAdvisor::blockYLength;

		MapBlock& mapBlock = MapAdvisor::mapBlocks[xBlock][yBlock];
		mapBlock.baseLocation = 1;		
	}

	// Update Choke Point Locations
	for(std::set<BWTA::Chokepoint*>::const_iterator i=BWTA::getChokepoints().begin();i!=BWTA::getChokepoints().end();i++)
	{
		Position pos = (*i)->getCenter();
		int x = pos.x();
		int y = pos.y();
		xBlock = x / (MapAdvisor::blockXLength * 32);
		yBlock = y / (MapAdvisor::blockYLength * 32);

		MapBlock& mapBlock = MapAdvisor::mapBlocks[xBlock][yBlock];
		mapBlock.chokePoint = 1;		
	}
	
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

	// TEMP
	MapAdvisor::initStratigicValues();

}

double MapAdvisor::getDistance(BWAPI::TilePosition initialTilePosition, BWAPI::TilePosition targetTilePosition, bool isFlyer)
{
	if (isFlyer)
		return initialTilePosition.getDistance(targetTilePosition);
	else
		// too slow to calculate
		// return BWTA::getGroundDistance(initialTilePosition, targetTilePosition);
		return initialTilePosition.getDistance(targetTilePosition);
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
		
		mapAdvisorLogFile << "\nBase Location :";
		for (int i = 0 ; i < MapAdvisor::blockXCount ; i++)
			mapAdvisorLogFile << "	" << MapAdvisor::mapBlocks[i][j].baseLocation;

		mapAdvisorLogFile << "\nUL Coordinates :";
		for (int i = 0 ; i < MapAdvisor::blockXCount ; i++)
			mapAdvisorLogFile << "	" << MapAdvisor::mapBlocks[i][j].upperLeftCoordinate.x << "," << MapAdvisor::mapBlocks[i][j].upperLeftCoordinate.y;
		
		mapAdvisorLogFile << "\nCtr Coordinates:";
		for (int i = 0 ; i < MapAdvisor::blockXCount ; i++)
			mapAdvisorLogFile << "	" << MapAdvisor::mapBlocks[i][j].mainCoordinate.x << "," << MapAdvisor::mapBlocks[i][j].mainCoordinate.y;
		mapAdvisorLogFile << "\n";
	}
	
}



bool MapAdvisor::isOccupiedPosition(Position target)
{
	UnitSet units = Broodwar->getUnitsOnTile(target.x(), target.y());
	return units.size() > 0;
}

/* BFS to find closest valid tile without Units */
Position MapAdvisor::getClosestPosition(Position target)
{
	typedef pair<int,int> pt;
	queue<pt> open;
	map<pt, bool> visited;

	// Start at target
	open.push(pt(target.x(), target.y()));

	while (!open.empty())
	{
		// Visited?
		if (visited[open.front()])
		{
			open.pop();
			continue;
		}

		// Mark visited
		pt p = open.front(); open.pop();
		visited[p] = true;

		// Prepare
		int x = p.first;
		int y = p.second;
		Position pos = Position(p.first, p.second).makeValid();

		// Goal test
		if (!isOccupiedPosition(pos))
		{
			return pos;
		}
		// Search
		else
		{
			open.push(pt(x + 1, y));
			open.push(pt(x - 1, y));
			open.push(pt(x, y + 1));
			open.push(pt(x, y - 1));
		}		
	}

	// If we couldn't find anything, try again later
	return target;
}

Position MapAdvisor::getPositionOutsideNearestChokepoint(BWAPI::Position p, int dist)
{
    BWTA::Chokepoint* chokepoint = BWTA::getNearestChokepoint(p);
	if( chokepoint != NULL ) {
		Position cp = chokepoint->getCenter();
		int cpx = cp.x();
		int cpy = cp.y();

		// Get a rough estimate of unit vector from Position to Chokepoint
		int dx = cpx - p.x();
		int dy = cpy - p.y();
		int d  = p.getApproxDistance(cp);  // close enough

		// Offset outward
		cpx += dx * dist / (1 + d);
		cpy += dy * dist / (1 + d);

		return Position(cpx, cpy).makeValid();
	} else {
		return Position();
	}
}
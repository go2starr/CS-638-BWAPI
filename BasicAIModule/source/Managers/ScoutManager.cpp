#include "ScoutManager.h"
#include <BWAPI.h>
#include <Common.h>
#include <Agent.h>
#include "Agents/State.h"
#include "MapAdvisor.h"


using namespace BWAPI;


//MapBlock MapAdvisor::mapBlocks[MapAdvisor::blockXCount][MapAdvisor::blockYCount];
//ScoutMapBlock ScoutManager::scoutMapBlocks[MapAdvisor::blockXCount][MapAdvisor::blockYCount];


void ScoutManager::onMatchStart()
{
	// check for file existance

	// delete if it exists
	
	// Open file
//	scoutManagerLogFile.open("scoutManager.log");
	// Write Intro line
//	scoutManagerLogFile << "Intro";
}

void ScoutManager::onMatchEnd()
{
	// close file
//	scoutManagerLogFile.close();
}

void ScoutManager::update()
{
    Manager::update();

	for (AgentSetIter it = agents.begin(); it != agents.end(); it++)
		(*it)->setState(ScoutState);

}


BWAPI::TilePosition ScoutManager::getScoutTilePosition(BWAPI::TilePosition tilePosition, bool isFlyer)
{
	double scoutValue = 0;
	double tempValue = 0;
	int x = 0;
	int y = 0;
	int xMapBlock = 0;
	int yMapBlock = 0;
	double stratigicValue;
	double distanceValue;
	double lastVisibleValue;


	bool reachable;


	BWAPI::TilePosition scoutTilePosition;
	BWAPI::TilePosition tempTilePosition;

	// Cal
	for(int i = 0 ; i < MapAdvisor::blockXCount ; i++)
		for (int j = 0 ; j < MapAdvisor::blockYCount ; j++)
		{
			tempTilePosition =  BWAPI::TilePosition(MapAdvisor::mapBlocks[i][j].mainCoordinate.x, MapAdvisor::mapBlocks[i][j].mainCoordinate.y);

			// Get the scouting value of the current block

			// Value based on the strategic value of the block
			stratigicValue = (MapAdvisor::mapBlocks[i][j].controlValue * ScoutManager::CONTROLVALUEVALUE);

			// Reduce the value for blocks that are further away
			distanceValue = (tilePosition.getDistance(tempTilePosition) / ScoutManager::DISTANCEVALUE);

			// Reduce the value of blocks that have been visited more recently
			lastVisibleValue = (Broodwar->getFrameCount() - MapAdvisor::mapBlocks[i][j].lastVisibileFrame) / ScoutManager::FRAMECOUNTVALUE;

			// Check to see if there is a path to the desired tile position
			reachable = (isFlyer || tilePosition.hasPath(tempTilePosition));

			tempValue = reachable * lastVisibleValue * (stratigicValue - distanceValue);

			// Recort the results for this block
			ScoutMapBlock& scoutMapBlock = ScoutManager::scoutMapBlocks[i][j];
			scoutMapBlock.stratigicValue = stratigicValue;
			scoutMapBlock.distanceValue = distanceValue;
			scoutMapBlock.lastVisibleValue = lastVisibleValue;
			scoutMapBlock.reachable = reachable;
			scoutMapBlock.selected = false;
			scoutMapBlock.Value = tempValue;

			if (tempValue > scoutValue)
			{
				scoutValue = tempValue;
				x = MapAdvisor::mapBlocks[i][j].mainCoordinate.x;
				y = MapAdvisor::mapBlocks[i][j].mainCoordinate.y;
				xMapBlock = i;
				yMapBlock = j;
			}
			


		}

	// Set destination tile position
	scoutTilePosition = BWAPI::TilePosition(x,y);

	// draw box around scout destination
	int xLeft = scoutTilePosition.x() * 32;
	int xRight = xLeft + 31;
	int yTop = scoutTilePosition.y() * 32;
	int yBottom = yTop + 31;
	int xUnit = tilePosition.x() * 32;
	int yUnit = tilePosition.y() * 32;

	BWAPI::Broodwar->drawBoxMap(xLeft, yTop, xRight, yBottom, BWAPI::Colors::Cyan, false);
	BWAPI::Broodwar->drawLineMap(xUnit, yUnit, xLeft, yTop, BWAPI::Colors::Cyan);
	
	// Log the results
	ScoutManager::scoutMapBlocks[xMapBlock][yMapBlock].selected = true;
	ScoutManager::log(xMapBlock, yMapBlock);

	return scoutTilePosition;

}

void ScoutManager::log(int x, int y){
	std::string writeString;




}


void ScoutManager::draw()
{
	Manager::draw();
}



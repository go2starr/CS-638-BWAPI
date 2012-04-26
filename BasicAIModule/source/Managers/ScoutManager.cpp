#include "ScoutManager.h"
#include <BWAPI.h>
#include <Common.h>
#include <Agent.h>
#include "Agents/State.h"
#include "MapAdvisor.h"

#include <windows.h>
#include <iostream>
#include <shlobj.h>

using namespace BWAPI;


#pragma comment(lib, "shell32.lib")



void ScoutManager::onMatchStart()
{
	// Get Path to My Documents
//	TCHAR my_documents[MAX_PATH];
//   HRESULT result SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, my_documents);

	TCHAR szPath[MAX_PATH];

if(SUCCEEDED(SHGetFolderPath(NULL, 
                             CSIDL_PERSONAL|CSIDL_FLAG_CREATE, 
                             NULL, 
                             0, 
							 szPath))){} 

	// check for file existance


	// delete if it exists
	
	// Open file
	scoutManagerLogFile.open("c:\\scoutManager.log");
	// Write Intro line
	scoutManagerLogFile << "Intro";
}

void ScoutManager::onMatchEnd()
{
	// close file
	scoutManagerLogFile.close();
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
			stratigicValue = (MapAdvisor::mapBlocks[i][j].stratigicValue * ScoutManager::CONTROLVALUEVALUE);

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
	if (Broodwar->getFrameCount() % 1000 == 0)
		ScoutManager::log(xMapBlock, yMapBlock);

	return scoutTilePosition;

}

void ScoutManager::log(int x, int y){
	scoutManagerLogFile << "\n\nFrame:	" ;
	scoutManagerLogFile << Broodwar->getFrameCount();
	scoutManagerLogFile << "\nSelected:	" << x << ", " << y;
	for (int j = 0 ; j < MapAdvisor::blockYCount ; j++)
	{
		scoutManagerLogFile << "\nMapBlock       :";
		for (int i = 0 ; i < MapAdvisor::blockXCount ; i++)
			scoutManagerLogFile << "	" << i << ", " << j;

		scoutManagerLogFile << "\nStratigic Value:";
		for (int i = 0 ; i < MapAdvisor::blockXCount ; i++)
			scoutManagerLogFile << "	" << ScoutManager::scoutMapBlocks[i][j].stratigicValue;

		scoutManagerLogFile << "\nVisible Value  :";
		for (int i = 0 ; i < MapAdvisor::blockXCount ; i++)
			scoutManagerLogFile << "	" << ScoutManager::scoutMapBlocks[i][j].lastVisibleValue;

		scoutManagerLogFile << "\nDistance Value :";
		for (int i = 0 ; i < MapAdvisor::blockXCount ; i++)
			scoutManagerLogFile << "	" << ScoutManager::scoutMapBlocks[i][j].distanceValue;
		
		scoutManagerLogFile << "\nReachable Value:";
		for (int i = 0 ; i < MapAdvisor::blockXCount ; i++)
			scoutManagerLogFile << "	" << ScoutManager::scoutMapBlocks[i][j].reachable;

		scoutManagerLogFile << "\nTotal Value    :";
		for (int i = 0 ; i < MapAdvisor::blockXCount ; i++)
			scoutManagerLogFile << "	" << ScoutManager::scoutMapBlocks[i][j].Value;


		scoutManagerLogFile << "\n";


		
	}


}


void ScoutManager::draw()
{
	Manager::draw();
}



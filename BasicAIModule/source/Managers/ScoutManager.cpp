#include "ScoutManager.h"
#include <BWAPI.h>
#include <Common.h>
#include <Agent.h>
#include "Agents/State.h"
#include "MapAdvisor.h"


using namespace BWAPI;

void ScoutManager::update()
{
    Manager::update();

	for (AgentSetIter it = agents.begin(); it != agents.end(); it++)
		(*it)->setState(ScoutState);

}

void ScoutManager::scratch()
{
	/*
	double scoutValue = 0;
	double tempValue = 0;
	int x = 0;
	int y = 0;

	BWAPI::TilePosition scoutTilePosition;
	BWAPI::TilePosition tempTilePosition;

	// Cal
	for(int i = 0 ; i < MapAdvisor::blockXCount ; i++)
		for (int j = 0 ; j < MapAdvisor::blockYCount ; j++)
		{
			tempTilePosition =  BWAPI::TilePosition(MapAdvisor::mapBlocks[i][j].mainCoordinate.x, MapAdvisor::mapBlocks[i][j].mainCoordinate.y);

			// Get the scouting value of the current block
			// Value based on the strategic value of the block
			tempValue = (MapAdvisor::mapBlocks[i][j].controlValue * ScoutManager::CONTROLVALUEVALUE);
			// Reduce the value for blocks that are further away
//			tempValue -= (tilePosition.getDistance(tempTilePosition) * ScoutManager::DISTANCEVALUE);
			// Reduce the value of blocks that have been visited more recently
			tempValue *= (Broodwar->getFrameCount() - MapAdvisor::mapBlocks[i][j].lastVisibileFrame) / ScoutManager::FRAMECOUNTVALUE;

			if (tempValue > scoutValue)
			{
				scoutValue = tempValue;
				x = MapAdvisor::mapBlocks[i][j].mainCoordinate.x;
				y = MapAdvisor::mapBlocks[i][j].mainCoordinate.y;
			}
			
		}
		scoutTilePosition = BWAPI::TilePosition(x, y);
	*/
	return ;//scoutTilePosition;
}


BWAPI::TilePosition ScoutManager::getScoutTilePosition(BWAPI::TilePosition tilePosition)
{
	double scoutValue = 0;
	double tempValue = 0;
	int x = 0;
	int y = 0;

	BWAPI::TilePosition scoutTilePosition;
	BWAPI::TilePosition tempTilePosition;

	// Cal
	for(int i = 0 ; i < MapAdvisor::blockXCount ; i++)
		for (int j = 0 ; j < MapAdvisor::blockYCount ; j++)
		{
			tempTilePosition =  BWAPI::TilePosition(MapAdvisor::mapBlocks[i][j].mainCoordinate.x, MapAdvisor::mapBlocks[i][j].mainCoordinate.y);

			// Get the scouting value of the current block
			// Value based on the strategic value of the block
			tempValue = (MapAdvisor::mapBlocks[i][j].controlValue * ScoutManager::CONTROLVALUEVALUE);
			// Reduce the value for blocks that are further away
			tempValue -= (tilePosition.getDistance(tempTilePosition) * ScoutManager::DISTANCEVALUE);
			// Reduce the value of blocks that have been visited more recently
			tempValue *= (Broodwar->getFrameCount() - MapAdvisor::mapBlocks[i][j].lastVisibileFrame) / ScoutManager::FRAMECOUNTVALUE;
			// Check to see if there is a path to the desired tile position


			if (tempValue > scoutValue)
			{
				scoutValue = tempValue;
				x = MapAdvisor::mapBlocks[i][j].mainCoordinate.x;
				y = MapAdvisor::mapBlocks[i][j].mainCoordinate.y;
			}
			
		}
		scoutTilePosition = BWAPI::TilePosition(x, y);

	return scoutTilePosition;

}

void ScoutManager::draw()
{
/*	Broodwar->drawTextScreen(2, 50, "\x10 SM : (SCV=%d)", numAgents(UnitTypes::Terran_SCV));*/
	Manager::draw();
}



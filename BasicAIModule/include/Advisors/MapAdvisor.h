#pragma once
/*
 *  MapManager.h
 *  
 *  Used to store data about the map
 *  Provies utility functions that inform other managers about the state of the map
 */
#include "Manager.h"



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


class MapManager : public Manager
{
private:

	int blockXCount;	// Number of blocks in the X direction
	int blockYCount;	// Number of blocks in the Y direction
	int blockXLength;	// Number of tiles in each block in the X direction
	int blockYLength;	// Number of tiles in each block in the Y direction
	int blockXOverShort;	// The number of tiles that the last set will be over/short in X
	int blockYOverShort;	// The number of tiles that the last set will be over/short in Y

    void drawDebugText();

public: 
	void init(int mapX, int mapY);
	void update();
	void draw();


    virtual const std::string& getName() const 
    {
        static const std::string name("MapMgr");
        return name; 
    }
};

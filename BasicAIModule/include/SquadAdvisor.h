#pragma once
/*
 * SquadAdvisor.h
 */
#include "Common.h"

#include <BWAPI.h>

class Agent;


class SquadAdvisor
{
public:

    static BWAPI::TilePosition getFarthestEnemyStartLocation(const BWAPI::TilePosition& pos);
    static BWAPI::TilePosition getClosestStartLocation(const BWAPI::TilePosition& pos);
    static BWAPI::TilePosition getNextStartLocation();
    
    static BWAPI::TilePosition findVulnerableBase();

    static bool isVisible(const BWAPI::TilePosition& pos
                        , const BWAPI::TilePosition& from=BWAPI::TilePosition(-1,-1));
    static bool canReach(const BWAPI::TilePosition& to, const BWAPI::TilePosition& from);
    static bool canReach( const Agent* agent, const BWAPI::TilePosition& pos );
};
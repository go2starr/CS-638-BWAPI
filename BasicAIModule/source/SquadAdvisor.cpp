/*
 * SquadAdvisor.cpp
 */
#include "SquadAdvisor.h"
#include "Agent.h"
#include "Common.h"

#include <BWAPI.h>
#include <BWTA.h>

#include <limits>


using BWAPI::Broodwar;
using BWAPI::Position;
using BWAPI::TilePosition;
using BWTA::BaseLocation;


TilePosition SquadAdvisor::getClosestStartLocation( const TilePosition& pos )
{
    double minDistance = std::numeric_limits<int>::max();
    TilePosition startLocation(pos);
    const BaseLocationSet& startLocations(BWTA::getStartLocations());
    BaseLocationSetConstIter it  = startLocations.begin();
    BaseLocationSetConstIter end = startLocations.end();
    for(; it != end; ++it)
    {
        const BaseLocation *baseLoc = *it;
        const TilePosition  basePos = baseLoc->getTilePosition();
        const double distance = pos.getDistance(basePos);
        if( distance < minDistance )
        {
            minDistance   = distance;
            startLocation = basePos;
        }
    }

    return startLocation;
}

TilePosition SquadAdvisor::getNextStartLocation()
{
    const BaseLocationSet& startLocations(BWTA::getStartLocations());
    BaseLocationSetConstIter it  = startLocations.begin();
    BaseLocationSetConstIter end = startLocations.end();
    for(; it != end; ++it)
    {
        const BaseLocation *baseLoc = *it;
        const TilePosition  basePos = baseLoc->getTilePosition();
        
        if( !isVisible(basePos) ) 
            return basePos;
    }  
    return TilePosition(-1,-1);
}

// TODO: this should be moved to a different advisor
// that can keep track of enemy buildings that have been discovered
TilePosition SquadAdvisor::findVulnerableBase()
{
    TilePosition baseLoc(-1,-1);

/* 
    for(int i = 0; i < (int)spottedBuildings.size(); ++i)
    {
        SpottedObject *obj = spottedBuildings.at(i);
        if( obj->getType().isResourceDepot() )
        {
            // TODO: remove this test
            if( !isDetectorCovering(obj->getTilePosition()) )
            {
                Broodwar->sendText("Possible vulnerable base @ (%d,%d)", 
                    obj->getTilePosition().x(), 
                    obj->getTilePosition().y())
                baseLoc = obj->getTilePosition();
            }
        }
    }

    if( baseLoc.x() < 0 || baseLoc.y() < 0 )
        Broodwar->printf("No vulnerable bases found...");
*/

    return baseLoc;
}

bool SquadAdvisor::isVisible( const TilePosition& pos, const TilePosition& from )
{
    if( !canReach(Broodwar->self()->getStartLocation(), pos) )
        return true;

    if( Broodwar->isVisible(pos) )
        return true;

    if( from != TilePosition(-1, -1) )
    {
        if( from.getDistance(pos) <= 3 )
            return true;
    }

    // TODO: add visited tile tracking to squads?

    return false;
}

bool SquadAdvisor::canReach(const TilePosition& to 
                                         , const TilePosition& from) 
{
   const int w = Broodwar->mapWidth();
   const int h = Broodwar->mapHeight();

   if( from.x() < 0 || from.x() >= w 
    || from.y() < 0 || from.y() >= h )
       return false;

   if( to.x() < 0 || to.x() >= w 
    || to.y() < 0 || to.y() >= h )
       return false;

   return from.hasPath(to);
}

bool SquadAdvisor::canReach(const Agent* agent
                          , const TilePosition& pos )
{
    return agent->getUnit().hasPath(Position(pos));
}

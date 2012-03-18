/*
 *  Agent.cpp
 */
#include "Agent.h"
#include "Manager.h"

#include <string>

#include <BWAPI.h>

using namespace BWAPI;
using std::string;


Agent::Agent(Unit& u)
    : unit(u)
    , state(IdleState)
    , unitTarget(NULL)
    , positionTarget()
    , unitTypeTarget(UnitTypes::None)
    , buildingReserved(false)
    , buildingLocation()
    , parentManager(NULL)
{ }

void Agent::printState()
{
	const int px = unit.getPosition().x();
	const int py = unit.getPosition().y();
	const int radius = unit.getRight() - px;

	// Draw owner, state, type
	if (state != GatherState)
		Broodwar->drawTextMap(px, py, "(%s, %s%s)", getParentManagerName().c_str(),
			StateStrings[state], unitTypeTargetValid() ? (string(", ") += string(UnitTypeStrings[unitTypeTarget.getID()])).c_str()
														:	"");

    // Note: drawing this stuff all the time clutters things up massively, but can be useful
    // TODO: look into adding a global chat flag to toggle this stuff (/debuginfo or something)
    if( 1 ) //unit.isSelected() )
    {
        Broodwar->drawCircleMap(px, py, radius, Colors::Yellow);

        // Draw line to position target if its valid
        if( positionTarget.isValid()
         && positionTarget != Positions::None 
         && positionTarget != Positions::Unknown 
         && positionTarget.x() != 0 && positionTarget.y() != 0 )
        {
            const int tx = positionTarget.x();
            const int ty = positionTarget.y();
            Broodwar->drawLineMap(px, py, tx, ty, Colors::Green);
            Broodwar->drawTextMap(tx, ty, "(%d, %d)", tx, ty);
        }

        // Draw a line to the unit target if its valid
        if( unitTarget != NULL )
        {
            const Position targetPos = unitTarget->getPosition();
            if( targetPos.isValid() 
             && targetPos != Positions::None 
             && targetPos != Positions::Unknown 
             && targetPos.x() != 0 && targetPos.y() != 0 )
            {
                const int tx = unitTarget->getPosition().x();
                const int ty = unitTarget->getPosition().y();
                Broodwar->drawLineMap(px, py, tx, ty, Colors::Red);
                Broodwar->drawTextMap(tx, ty, "id: %x", unitTarget);
            }
        }
    }
}

bool Agent::operator==(const Agent& other)
{
	// Compare by pointer values
	if( &(this->unit) == &(other.unit) )
		return true;
	else
		return false;
}

void Agent::update()
{
    if( state == BuildState )
        positionTarget = Position(buildingLocation);

	printState();
}

void Agent::setParentManager(Manager *manager)
{
    if( manager != NULL ) 
        parentManager = manager;
}

const std::string Agent::getParentManagerName() const 
{
    return (parentManager != NULL) ? 
        parentManager->getName() : "None";
}

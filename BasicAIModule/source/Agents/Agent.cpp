/*
 *  Agent.cpp
 */
#include "Agent.h"

#include <BWAPI.h>

using namespace BWAPI;


Agent::Agent(Unit& u)
    : unit(u)
    , state(IdleState)
    , unitTarget(NULL)
    , positionTarget()
    , unitTypeTarget(UnitTypes::None)
    , buildingReserved(false)
    , buildingLocation()
{ }

void Agent::printState() const
{
	const int px = unit.getPosition().x();
	const int py = unit.getPosition().y();
	const int radius = unit.getRight() - px;

    // Always draw state 
	Broodwar->drawTextMap(px, py, "State: %s", StateStrings[state] );

    // Draw UnitTypeTarget info if it is valid
    const int typeTargetID = unitTypeTarget.getID();
    if( typeTargetID >= 0 && typeTargetID < NUM_UNIT_TYPES )
        Broodwar->drawTextMap(px, py + 10, "type-target: %s", UnitTypeStrings[typeTargetID]);

    // Note: drawing this stuff all the time clutters things up massively
    // TODO: look into adding a chat flag to toggle this stuff (/debuginfo or something)
    if( unit.isSelected() )
    {
        Broodwar->drawCircleMap(px, py, radius, Colors::Yellow);

        // Draw position target 
        const int posOffset = 20;
        Broodwar->drawTextMap(px, py + posOffset, "pos-target: (%d,%d)", 
            positionTarget.x(), positionTarget.y());

        // Draw a line to the unit target if its valid
        if( unitTarget != NULL )
        {
            const int tx = unitTarget->getPosition().x();
            const int ty = unitTarget->getPosition().y();
            Broodwar->drawLineMap(px, py, tx, ty, Colors::Red);
            Broodwar->drawTextMap(tx, ty, "id: %x", unitTarget);
        }
    }

//	if( unit.isSelected() )
//        Broodwar->drawTextMap(px, py + 30, "unit id: %x", &unit);
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
	printState();
}

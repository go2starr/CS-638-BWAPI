/*
 *  Agent.cpp
 */
#include "Agent.h"

#include <BWAPI.h>

using namespace BWAPI;


Agent::Agent(Unit& u)
    : unit(u)
    , state(IdleState)
    , buildingReserved(false)
{ }

void Agent::printState() const
{
	const int px = unit.getPosition().x();
	const int py = unit.getPosition().y();
	const int radius = unit.getRight() - px;

    // Always draw state 
	Broodwar->drawCircleMap(px, py, radius, Colors::Yellow);
	Broodwar->drawTextMap(px, py, "State: %s", StateStrings[state] );

    // Draw UnitTypeTarget info if it is valid
    const int typeTargetID = unitTypeTarget.getID();
    if( typeTargetID >= 0 && typeTargetID < NUM_UNIT_TYPES )
        Broodwar->drawTextMap(px, py + 10, "type-target: %s", UnitTypeStrings[typeTargetID]);

    // Note: drawing this stuff all the time clutters things up massively
    // TODO: look into adding a chat flag to toggle this stuff (/debuginfo or something)
    if( unit.isSelected() )
    {
        // Draw position target 
        const int posOffset = 20;
        Broodwar->drawTextMap(px, py + posOffset, "pos-target: (%d,%d)", 
            positionTarget.x(), positionTarget.y());

        // Draw unit target address if its valid
        // (calculate position based on whether other stuff is drawn)
        int unitOffset = -1;
        if( unitTarget != NULL )
            unitOffset = (posOffset == -1) ? 20 : posOffset + 10;
        if( unitOffset != -1 )
            Broodwar->drawTextMap(px, py + unitOffset, "unit-target: %x", unitTarget);
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

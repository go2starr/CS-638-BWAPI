/*
 *  Agent.cpp
 */
#include "Agent.h"

#include <BWAPI.h>

using namespace BWAPI;


Agent::Agent(Unit& u)
: unit(u), state(IdleState), buildingReserved(false)
{ }

void Agent::printState() const
{
	if( unit.isSelected() )
	{
		const int px = unit.getPosition().x();
		const int py = unit.getPosition().y();
		const int radius = unit.getRight() - px;

		Broodwar->drawCircleMap(px, py, radius, Colors::Yellow);

		// FIXME: when an agent changes states, 
		// the new state string draws over the 
		// old one for some reason...
		Broodwar->drawTextMap(px, py, "State: %s", StateStrings[state] );
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
	printState();
}

/*
 *  Agent.cpp
 */
#include "Agent.h"

Agent::Agent(Unit& u)
	: unit(u), state()
{ }

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

}
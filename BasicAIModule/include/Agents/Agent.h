/*
 *  Agent.h 
 */
#pragma once
#include <BWAPI.h>
#include "States/State.h"

using namespace BWAPI;

class Agent 
{
protected:
  Unit &unit;					// The Unit this Agent controls
  State *state;					// The State this Agent is in

public:  
	/*
	 * update - Called on each frame to update this agent's state and unit.
	 */
	virtual void update();
 
	bool operator==(const Agent& other);

	// attr_accessors
	inline const State* getState() const { return state; }
	inline const Unit& getUnit() const { return unit; }

protected:
	// C'tor (must be called from subclass)
	Agent(Unit &u);
};

/*
 *  Agent.h 
 */
#pragma once
#include <BWAPI.h>
#include <State.h>

using namespace BWAPI;

class Agent 
{
protected:
  Unit &unit;					// The Unit this Agent controls
  State state;					// The State this Agent is in
  Unit *unitTarget;             // Targeted unit (for gather, attack, etc.)
  Position *positionTarget;     // Targeted position (for moving, evade, etc.)

public:  
	/*
	 * update - Called on each frame to update this agent's state and unit.
	 */
	virtual void update();
 
	bool operator==(const Agent& other);

	// attr_accessors
	inline void setState(State state) { this->state = state; }
	inline const State getState() const { return state; }
	inline const Unit& getUnit() const { return unit; }

protected:
	// C'tor (must be called from subclass)
	Agent(Unit &u);
};

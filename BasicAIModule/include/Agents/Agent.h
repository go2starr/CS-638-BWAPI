/*
 *  Agent.h 
 */
#pragma once
#include "State.h"

#include <BWAPI.h>


class Agent 
{
protected:
	BWAPI::Unit &unit;                      // The Unit this Agent controls
	State state;                            // The State this Agent is in

	BWAPI::Unit *unitTarget;                // Targeted unit (for gather, attack, etc.)
	BWAPI::Position *positionTarget;        // Targeted position (for moving, evade, etc.)
	BWAPI::UnitType unitTypeTarget;         // Targeted unit type (for production of units, buildings, etc.)

	bool buildingReserved;                  // Have we reserved space for a building yet?
	BWAPI::TilePosition buildingLocation;	// Location to build target building at

public:  
	/*
	 * update - Called on each frame to update this agent's state and unit.
	 */
	virtual void update();
 
	bool operator==(const Agent& other);

	// attr_accessors
	inline void setUnitTarget(BWAPI::Unit *target)         { unitTarget = target; }
	inline void setPositionTarget(BWAPI::Position *target) { positionTarget = target; }
	inline void setUnitTypeTarget(BWAPI::UnitType target)  { unitTypeTarget = target; }
	inline void setState(State state)                      { this->state = state; }
	inline const State getState()       const { return state; }
	inline const BWAPI::Unit& getUnit() const { return unit; }
	inline BWAPI::Unit& getUnit() { return unit; }

protected:
	// C'tor (must be called from subclass)
	Agent(BWAPI::Unit &u);

	virtual void printState() const;
};

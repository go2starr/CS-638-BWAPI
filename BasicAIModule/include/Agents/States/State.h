/*
 *  State.h
 */
#pragma once
#include "Target.h"

class State
{
public:
	enum StateId
	{
		ActorState,
		AddonState,
		AttackState,
		BuildingState,
		DefendState,
		EvadeState,
		GatherState,
		IdleState,
		ResearchState,
		ScoutState,
		StructureState,
		TrainingState,
		UpgradeState
	};

protected:
	Target target;      // Current target
  
public:
	// C'tor
	State() : target() { }
	State(const Target& target)
		: target(target)
	{ }
	State(const State& state)
		: target(state.target)
	{ }
	// D'tor
	virtual ~State(){}

	// State entry
	virtual void onEnter();
	// State update
	virtual void onUpdate();
	// State exit
	virtual void onExit();
	
	// attr_accessor
	inline void setTarget(const Target& t) { target = t;    }
	inline const Target& getTarget() const { return target; }

	// Enumeration
	virtual StateId getId()=0;
};

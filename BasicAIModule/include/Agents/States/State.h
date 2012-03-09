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
	Target *target;      // Current target
  
public:
	// C'tor
	State() : target() { }
	State(Target *target)
		: target(target)
	{ }

	/*
	State(const State& state)
		: target(state.target)
	{ }
	*/

	// D'tor
	virtual ~State(){}

	// State entry
	virtual void onEnter();
	// State update
	virtual void onUpdate();
	// State exit
	virtual void onExit();
	
	// attr_accessor
	inline void setTarget(Target *t) { target = t; }
	inline const Target* getTarget() const { return target; }

	// Enumeration
	virtual StateId getId()=0;
};


// TODO : What is a better way/place of including all states?
#include "ActorState.h"
#include "AddonState.h"
#include "AttackState.h"
#include "BuildingState.h"
#include "DefendState.h"
#include "EvadeState.h"
#include "GatherState.h"
#include "IdleState.h"
#include "ResearchState.h"
#include "ScoutState.h"
#include "StructureState.h"
#include "TrainingState.h"
#include "UpgradeState.h"
/*
 *  State.h
 */
#pragma once


enum State
{
	AttackState = 0,
	BuildState,
	BuildingState,
	DefendState,
	EvadeState,
	GatherState,
	IdleState,
	ResearchState,
	ScoutState,
	TrainState,
	UpgradeState, 
    NUM_STATES
};

extern const char* StateStrings[];

extern const char* UnitTypeStrings[];
extern const unsigned char NUM_UNIT_TYPES;

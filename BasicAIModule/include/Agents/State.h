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
	UpgradeState
};

extern const char* StateStrings[];
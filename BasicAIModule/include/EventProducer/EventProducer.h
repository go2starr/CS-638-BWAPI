/*
 *  EventProducer.h  -  Analyses the game state and throws Events which are of interest
 */
#pragma once
#include "Strategizer.h"
#include "GameStateDB.h"
#include "GameEvent.h"


class EventProducer
{
private:
	Strategizer& strategizer;
	GameStateDB& gsdb;

public:
	void update();

};

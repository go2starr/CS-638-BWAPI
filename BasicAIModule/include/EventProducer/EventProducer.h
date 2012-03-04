/*
 *  EventProducer.h  -  Analyses the game state and throws Events which are of interest
 */
#pragma once
#include "../Strategizer.h"
#include "../GameStateDB.h"
#include "Event.h"

class EventProducer
{
  Strategizer &strategizer;
  GameStateDB &gsdb;

public:
  void update();

};

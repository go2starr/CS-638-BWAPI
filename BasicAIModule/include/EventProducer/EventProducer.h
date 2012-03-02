/*
 *  EventProducer.h  -  Analyses the game state and throws Events which are of interest
 */
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

/*
 *  Strategizer.cpp
 */
#include "Strategizer.h"
#include "UnitAgents/SCVAgent.h"
#include "BWAPI.h"

using namespace BWAPI;
using namespace std;


/* update - Called by AI Module for each frame */
void Strategizer::update()
{

}

/* onMatchStart - Called by AI Module when a new match begins */
void Strategizer::onMatchStart()
{
	Broodwar->sendText("Hello!");
 
	set<Unit*> units = Broodwar->self()->getUnits();
	set<Unit*>::iterator i = units.begin();

	Broodwar->sendText("Units include: ");
	for (; i != units.end(); ++i)
	{
		Broodwar->sendText("%s", (*i)->getType().getName().c_str());
		SCVAgent a(*(*i));
	}
}

/* onEvent - Called by EventProducer when a new event is detected */
void Strategizer::onEvent(JohnConnorBot::Event &e)
{

}



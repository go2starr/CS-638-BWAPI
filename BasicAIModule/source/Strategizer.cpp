/*
 *  Strategizer.cpp
 */
#include "Strategizer.h"
#include "UnitAgents/SCVAgent.h"
#include "BWAPI.h"

using namespace BWAPI;
using namespace std;

void Strategizer::update()
{

}

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
		a.setState(GatherState());
	}
}

void Strategizer::onEvent(JohnConnorBot::Event &e)
{

}



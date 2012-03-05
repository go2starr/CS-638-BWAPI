/*
 *  Strategizer.cpp
 */
#include "Strategizer.h"
#include "BWAPI.h"

using namespace BWAPI;
using namespace std;

void Strategizer::update()
{
	Broodwar->sendText("Hello!");
 
	set<Unit*> units = Broodwar->self()->getUnits();
	set<Unit*>::iterator i = units.begin();

	Broodwar->sendText("Units include: ");
	for (; i != units.end(); ++i)
	{
		Broodwar->sendText("%s", (*i)->getType().getName().c_str());
<<<<<<< HEAD:BasicAIModule/Source/BasicAIModule/Strategizer.cpp
		SCVAgent a(*(*i));
=======
		SCVAgent(*i);
>>>>>>> e48dc40d6fe58cad96c27f81df724f3e75afa764:BasicAIModule/source/Strategizer.cpp
	}
}

void Strategizer::onEvent(JohnConnorBot::Event &e)
{
}



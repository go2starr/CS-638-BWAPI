#include "ScoutManager.h"
#include <BWAPI.h>
#include <Common.h>
#include <Agent.h>
#include "Agents/State.h"


using namespace BWAPI;

void ScoutManager::update()
{
    Manager::update();

	for (AgentSetIter it = agents.begin(); it != agents.end(); it++)
		(*it)->setState(ScoutState);

}

void ScoutManager::draw()
{
	Broodwar->drawTextScreen(2, 50, "\x10 SM : (SCV=%d)", numAgents(UnitTypes::Terran_SCV));
	Manager::draw();
}
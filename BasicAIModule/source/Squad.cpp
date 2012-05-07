/*
* Squad.cpp
*/
#include "Squad.h"
#include "Agent.h"
#include "Common.h"
#include "DebugFlags.h"

#include <algorithm>
#include <cassert>
#include <limits>

using namespace BWAPI;

using std::string;
using std::pair;
using std::min;
using std::max;



const char* SquadTypeStrings[] = { "attack", "defend", "explore", "bunker" };


/************************************************************************/
/* Squad class
/************************************************************************/
int Squad::nextAvailableId = 1;

Squad::Squad(const string& name, 
			 const SquadType& type, 
			 const int priority)
			 : name(name)
			 , id(nextAvailableId++)
			 , priority(priority)
			 , active(false)
			 , leader(NULL)
			 , agents()
			 , type(type)
			 , composition()
			 , bunkerTarget(NULL)
{ }

void Squad::update()
{

	if ((int)agents.size() == 0)
		return;

	if (leader == NULL) 
	{
		leader = *agents.begin();
	}

	// Find a new leader if current one dies
	if (leader->getUnit().getHitPoints() <= 0)
	{
		removeAgent(leader);
		for (AgentSetIter it = agents.begin(); it != agents.end(); it++)
		{
			leader = *it;
			break;
		}
	}
	// Remove dead agents
	for (AgentSetIter it = agents.begin(); it != agents.end(); it++) {
		if ((*it)->getUnit().getHitPoints() <= 0)
			removeAgent(*it);
	}

	// Regather every so often
	if( getRadius() > 100  && Broodwar->getFrameCount() % 10000 == 0 )
	{
		// gatherTogether();
	}
	else
	{
		// Do what the leader is doing
		AgentSetIter it  = agents.begin();
		AgentSetIter end = agents.end();
		for(; it != end; ++it)
		{
			Agent *agent = *it;

			// command bunker squads to load into bunkers
			// TODO: check if already loaded
			if (type == bunker && bunkerTarget != NULL)
			{
				agent->getUnit().load(bunkerTarget);
			}

			// set non-leaders in squad
			if( agent != leader ) 
			{
				// agent->setState(leader->getState());
				if (type == attack)
				{
					agent->setState(AttackState);
				}
				agent->setPositionTarget(leader->getPositionTarget());
				agent->setUnitTarget(leader->getUnitTarget());
				agent->setUnitTypeTarget(leader->getUnitTypeTarget());
			}
		}
	}
}

void Squad::draw()
{
	if ((int)agents.size() == 0)
		return;

	if( DebugFlags::instance().getFlag(none) )
		return;

	if( !DebugFlags::instance().getFlag(squad_details) )
		return;

	// Draw  a circle around the group
	const Position center = getCenter();
	const int groupRadius = getRadius();
	Broodwar->drawCircleMap(center.x(), center.y(), groupRadius, Colors::Grey);

	// Draw a circle around the leader
	const Unit& leaderUnit = leader->getUnit();
	const int leaderX = leaderUnit.getPosition().x();
	const int leaderY = leaderUnit.getPosition().y();
	const int radius = leaderUnit.getRight() - leaderUnit.getPosition().x();
	Broodwar->drawCircleMap(leaderX, leaderY, radius + 2, Colors::Red);
	Broodwar->drawCircleMap(leaderX, leaderY, radius - 2, Colors::Orange);

	BWAPI::Unit * unitTarget;
	// Draw squad leader enemy target
	if ((unitTarget = leader->getUnitTarget()) != NULL)
	{
		const int enemyX = unitTarget->getPosition().x();
		const int enemyY = unitTarget->getPosition().y();
		const int enemyRadius = unitTarget->getRight() - unitTarget->getPosition().x();
		Broodwar->drawCircleMap(enemyX, enemyY, enemyRadius + 2, Colors::Green);
		Broodwar->drawCircleMap(enemyX, enemyY, enemyRadius - 2, Colors::Yellow);
	}

	// Draw lines from Agents to their leader
	// 	for (AgentSetIter it = agents.begin(); it != agents.end(); it++)
	// 	{
	// 		int agentX = (*it)->getUnit().getPosition().x();
	// 		int agentY = (*it)->getUnit().getPosition().y();
	// 		Broodwar->drawLineMap(agentX, agentY, leaderX, leaderY, Colors::Teal);
	// 	}
}

void Squad::addRequirement(const int numDesired, const UnitType& unitType)
{
	composition.push_back(SquadComposition(numDesired, unitType));
}

void Squad::addAgent(Agent* agent)
{
	agents.insert(agent);
}

void Squad::moveAgent(Agent* agent, Squad * toSquad)
{
	// if not found, return
	if (agents.find(agent) == agents.end())
		return;

	removeAgent(agent);
	toSquad->addAgent(agent);

	// check for leaders
	if (leader == agent) {
		if ((int)agents.size() > 0) {
			leader = *agents.begin();
		}
		else {
			leader = NULL;
		}
	}

	if (toSquad->getLeader() == NULL) {
		toSquad->setLeader(agent);
	}
}

void Squad::setLeader(Agent* agent)
{
	if( leader == agent ) 
		return;
	if( !isAssigned(agent) )
		addAgent(agent);
	leader = agent;
}

Position Squad::getCenter()
{
	if( agents.empty() )
		return Position(-1, -1);

	int x0, y0, x1, y1;
	x0 = y0 = std::numeric_limits<int>::max();
	x1 = y1 = std::numeric_limits<int>::min();

	AgentSetConstIter it  = agents.begin();
	AgentSetConstIter end = agents.end();
	for(; it != end; ++it)
	{
		const Unit& unit = (*it)->getUnit();
		const int x = unit.getPosition().x();
		const int y = unit.getPosition().y();

		if( x < x0 && x >= 0 ) x0 = x;
		if( y < y0 && y >= 0 ) y0 = y;
		if( x > x1 && x >= 0 ) x1 = x;
		if( y > y1 && y >= 0 ) y1 = y;
	}

	return Position(x0 + ((x1 - x0) / 2)
		, y0 + ((y1 - y0) / 2));
	/*
	int sumX, sumY;
	int x0, y0;
	sumX = sumY = 0;
	x0 = y0 = 0;

	for (AgentSetIter it = agents.begin(); it != agents.end(); it++)
	{
	int x = (*it)->getUnit().getPosition().x();
	int y = (*it)->getUnit().getPosition().y();
	sumX += x;
	sumY += y;
	}

	if (agents.size())
	{
	x0 = sumX / agents.size();
	y0 = sumY / agents.size();
	}
	return pt(x0, y0);
	*/
}

int Squad::getRadius()
{
	Position center(getCenter());
	int r = 0;
	for (AgentSetConstIter it = agents.begin(); it != agents.end(); it++)
	{
		const Unit& unit = (*it)->getUnit();
		r = (int)max(r, unit.getDistance(center));
	}
	return r;
}

void Squad::gatherTogether()
{
	const Position center(getCenter());
	if( center.x() < 0 || center.y() < 0 ) 
		return;

	AgentSetIter it  = agents.begin();
	AgentSetIter end = agents.end();
	for(; it != end; ++it)
	{
		Agent *agent = *it;
		Unit  &unit  = agent->getUnit();
		if( agent == leader )
			unit.stop();
		else
		{
			const Unit& leaderUnit = leader->getUnit();
			// set this so if in attack state and agent gets updated 
			// it doesn't keep going to enemey base and when radius is
			// low again it's position target will get set to the 
			// leader's again
			agent->setPositionTarget(leaderUnit.getPosition());
			unit.move(leaderUnit.getPosition());
		}
	}
}

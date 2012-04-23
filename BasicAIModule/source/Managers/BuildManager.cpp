/*
 *  BuildManager.cpp
 *  
 *  Used to produce new units, buildings and add-ons
 */
#include "BuildManager.h"
#include "ResourceAdvisor.h"
#include "Common.h"
#include "Agent.h"
#include "State.h"

#include <BWAPI.h>
#include <BWTA.h>

#include <deque>
#include <map>

using namespace BWAPI;

using std::deque;
using std::map;


void BuildManager::update()
{
	// Update Agents
	Manager::update();

	/*** <DEBUG> ****/
	if (buildStack.size() > 50 || 
		buildQueue.size() > 50)
	{
		Broodwar->sendText("BM STACK OVERFLOW");
		return;
	}
	/*** </DEBUG> ****/

	// Done with the current request?
	if (buildStack.empty())
	{
		// Try to start a new request
		if (!buildQueue.empty())
		{
			buildStack.push(buildQueue.front());
			buildQueue.pop();
		}
	}

	// Process requests if there are any
	if (buildStack.empty())
		return;

	BuildReq &req = buildStack.top();
	UnitType type = req.type;

	switch (req.state)
	{
	case NEW: 
	{
		//Broodwar->sendText("Trying to build: %s", type.c_str());

		// Check reqs
		int mineralsNeeded = type.mineralPrice();
		int gasNeeded = type.gasPrice();
		int mineralsOwned = Broodwar->self()->minerals();
		int gasOwned = Broodwar->self()->gas();

		// Required supply?
		int supplyOwned = Broodwar->self()->supplyTotal() - Broodwar->self()->supplyUsed();
		if (supplyOwned < type.supplyRequired())
		{
			break;
		}

		// Required units?
		map<UnitType, int> requiredUnits = type.requiredUnits();
		bool unitsFound = true;
		for (map<UnitType, int>::iterator i = requiredUnits.begin(); i != requiredUnits.end(); i++)
		{
			UnitType rt = (*i).first;
			int rc = (*i).second;
			if (Broodwar->self()->visibleUnitCount(rt) < 1)
			{
				build(rt, true);
				unitsFound = false;
			}
		}
		if (!unitsFound)
			break;
		
		// Required builder?
		UnitType builderType = type.whatBuilds().first;
		if (Broodwar->self()->visibleUnitCount(builderType) < 1)
		{
			build(builderType, true);
			break;
		}

		// Required resources?
		if (mineralsOwned < mineralsNeeded ||
			gasOwned < gasNeeded) 
		{
			break;
		}

		// OK - requirements met, let's build it!
		// Find an idle builder
		for (AgentSetIter i = agents.begin(); i != agents.end(); i++)
		{
			if ((*i)->getUnit().getType().getID() == builderType.getID()
				&& (*i)->getState() != BuildState 
				&& (*i)->getState() != TrainState)  
			{
				// Found one, build it (this works for addons too)
				if (type.isBuilding())
					(*i)->setState(BuildState);
				else
					(*i)->setState(TrainState);
				(*i)->setUnitTypeTarget(type);
				// Remember we issued the command
				req.builder = &(*i)->getUnit();
				req.state = ISSUED;
				Broodwar->sendText("BM: Worker found, building %s", type.c_str());
				break;
			}
		}
		// Wait if we didn't find a builder
		if (req.builder == NULL)
		{
			break;
		}
	}
	break;

	case ISSUED:
	{
		/* We need to determine if the order has been started (i.e. resources spent) */
		Unit* builder = req.builder;

		if (builder->getType().isBuilding())
		{
			if (builder->isTraining() ||    // Train units
				builder->isConstructing())  // Addon
			{
				req.state = STARTED;
			}
		}
		else if (builder->getType().isWorker())
		{
			/*
			 * We can't check if a builder is *really* building using ->isConstructing(),
			 * but we can see if a structure is being built by our builder
			 */
			UnitSet ownedUnits = Broodwar->self()->getUnits();
			for (UnitSetConstIter i = ownedUnits.begin(); i != ownedUnits.end(); i++)
			{
				if ((*i)->getBuildUnit() == builder &&
					(*i)->isVisible())
				{
					req.state = STARTED;
				}
			}
		}
		break;
	}

	case STARTED:
		/*
		 * TODO:  Monitor the orders we have started, and verify them to completion.
		 *        In the case of a killed SCV doing a build, we may want to restart the
		 *        order.
		 */
		buildStack.pop();
		break;

	case COMPLETE:
		break;
	}

    // Keep our workers busy when they aren't building
    AgentSet workers(getAgentsOfType(UnitTypes::Terran_SCV));
    for(AgentSetIter worker = workers.begin(); worker != workers.end(); ++worker)
    {
        Agent& agent = **worker;
        if( agent.getState() == IdleState ) 
        {
            ResourceAdvisor::makeAgentGatherMinerals(agent);
        }
    }
}

void BuildManager::build(UnitType type, bool immediate)
{
	TilePosition defaultLocation = Broodwar->self()->getStartLocation();
	build(type, defaultLocation, immediate);
}

void BuildManager::build(UnitType type, TilePosition goalPosition, bool immediate)
{
	BuildReq req;
	// Internal
	req.type = type;
	req.goalPosition = goalPosition;
	req.count = 1;
	// External
	req.state = NEW;
	req.builder = NULL;

	// DEBUG //
	Broodwar->sendText("BM: Rcvd order for %s", type.c_str());

	if (immediate) 
		buildStack.push(req);
	else
		buildQueue.push(req);
}

// Note: this is ugly because in order to print its elements 
// we have to break down the queue and stack and rebuild them
// in the right order when we're done
void BuildManager::drawDebugText()
{
    const int starty  = 60;
    int x = 2; 
    int y = starty;

    Broodwar->drawTextScreen(x, y, "BuildMgr - queue (%d)", (int)buildQueue.size());

    deque<BuildReq> tempq;
    while(!buildQueue.empty())
    {
        BuildReq req = buildQueue.front();
        buildQueue.pop();
        Broodwar->drawTextScreen(x, y += 10, "-%s", UnitTypeStrings[req.type.getID()]);
        tempq.push_back(req);
    }
    while(!tempq.empty())
    {
        BuildReq req = tempq.front();
        tempq.pop_front();
        buildQueue.push(req);
    }

    tempq.clear();

    x = 150;
    y = starty;

    Broodwar->drawTextScreen(x, y, "BuildMgr - stack (%d)", (int)buildStack.size());
    while(!buildStack.empty())
    {
        BuildReq req = buildStack.top();
        buildStack.pop();
        Broodwar->drawTextScreen(x, y += 10, "-%s", UnitTypeStrings[req.type.getID()]);
		tempq.push_front(req);
    }
    while(!tempq.empty())
    {
        BuildReq req = tempq.front();
        tempq.pop_front();
        buildStack.push(req);
    }
}

void BuildManager::draw()
{
	Manager::draw();
}

/*
 *  BuildManager.cpp
 *  
 *  Used to produce new units, buildings and add-ons
 *  Passes tasks down to the Construction or Production Managers
 */
#include "BuildManager.h"
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
	Broodwar->drawTextScreen(2, 40, "\x07 BM : (SCV=%d) (CC=%d)", 
		numAgents(UnitTypes::Terran_SCV),
		numAgents(UnitTypes::Terran_Command_Center));

    drawDebugText();

	// Update Agents
	Manager::update();

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

		// Required resources?
		if (mineralsOwned < mineralsNeeded ||
			gasOwned < gasNeeded) 
		{
			//Broodwar->sendText("BM: %d/%d minerals %d/%d gas for %s",
			//	mineralsOwned, mineralsNeeded, gasOwned, gasNeeded, type.c_str());
			return;
		}

		// Required supply?
		int supplyOwned = Broodwar->self()->supplyTotal() - Broodwar->self()->supplyUsed();
		if (supplyOwned < type.supplyRequired())
		{
			Broodwar->sendText("BM: Insufficient supply (%d/%d) for %s, building Supply Depot", 
				supplyOwned, type.supplyRequired(), type.c_str());	
			build(UnitTypes::Terran_Supply_Depot, true);
			return;
		}

		// Required units?
		map<UnitType, int> requiredUnits = type.requiredUnits();
		for (map<UnitType, int>::iterator i = requiredUnits.begin(); i != requiredUnits.end(); i++)
		{
			UnitType rt = (*i).first;
			int rc = (*i).second;
			if (Broodwar->self()->allUnitCount(rt) < rc)
			{
				build(rt, true);
				Broodwar->sendText("%s pushed: prereq to build %s", rt.c_str(), type.c_str());
				return;
			}
		}
		
		// Required builder?
		UnitType builderType = type.whatBuilds().first;
		if (Broodwar->self()->allUnitCount(builderType) < 1)
		{
			build(builderType, true);
			Broodwar->sendText("%s pushed:  Need a %s to build %s", builderType.c_str(), type.c_str());
			return;
		}
		// OK - requirements met, let's build it!
		if (!type.isAddon())
		{
			// Find an idle builder
			for (AgentSetIter i = agents.begin(); i != agents.end(); i++)
			{
				if ((*i)->getUnit().getType().getID() == builderType.getID() //)
                 && (*i)->getState() == IdleState)  // NOTE: structures don't transition properly out of TrainState into IdleState
                                                    // so this check will fail when it probably shouldn't
				{
					// Found one, build it
					if (type.isBuilding())
						(*i)->setState(BuildState);
					else
						(*i)->setState(TrainState);
					(*i)->setUnitTypeTarget(type);

					// Remember we issued the command
					req.builder = &(*i)->getUnit();
					req.state = ISSUED;

					// DEBUG //
					Broodwar->sendText("BM: Worker found, building %s", type.c_str());
				}
			}
			// Wait if we didn't find a builder
			if (req.builder == NULL)
			{
				//Broodwar->sendText("BM: Could not find worker of type %s for %s", builderType.c_str(), 
				//	type.c_str());
				break;
			}
		}
		else
		{
			Broodwar->sendText("!! Bad build order: addons not yet implemented");
		}
		break;
	}

	case ISSUED:
	{
		//Broodwar->sendText("BM: ===== ISSUED =====");

		/* We need to determine if the order has been started (i.e. resources spent) */
		Unit* builder = req.builder;

		if (builder->getType().isBuilding())
		{
			if (builder->isTraining())
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
				if ((*i)->getBuildUnit() == builder)
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
        tempq.push_back(req);
    }
    while(!tempq.empty())
    {
        BuildReq req = tempq.front();
        tempq.pop_front();
        buildStack.push(req);
    }
    
}

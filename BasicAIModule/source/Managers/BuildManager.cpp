#include "BuildManager.h"
#include <BWTA.h>

#include <set>
#include <map>
using std::set;
using std::map;

void BuildManager::update()
{
	BWAPI::Broodwar->drawTextScreen(2, 50, "\x1F BM : (SCV=%d) (CC=%d)", 
		numAgents(BWAPI::UnitTypes::Terran_SCV),
		numAgents(BWAPI::UnitTypes::Terran_Command_Center));

	/* 
	 * TODO:  Normally, we would pass down orders to the specialized builders.
	 *        However, to get things rolling I'm doing it here
	 */

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
	BWAPI::UnitType type = req.type;

	switch (req.state)
	{
	case NEW: 
	{
		//BWAPI::Broodwar->sendText("Trying to build: %s", type.c_str());

		// Check reqs
		int mineralsNeeded = type.mineralPrice();
		int gasNeeded = type.gasPrice();
		int mineralsOwned = BWAPI::Broodwar->self()->minerals();
		int gasOwned = BWAPI::Broodwar->self()->gas();

		// Required resources?
		if (mineralsOwned < mineralsNeeded ||
			gasOwned < gasNeeded) 
		{
			//BWAPI::Broodwar->sendText("BM: %d/%d minerals %d/%d gas for %s",
			//	mineralsOwned, mineralsNeeded, gasOwned, gasNeeded, type.c_str());
			return;
		}

		// Required supply?
		int supplyOwned = BWAPI::Broodwar->self()->supplyTotal() - BWAPI::Broodwar->self()->supplyUsed();
		if (supplyOwned < type.supplyRequired())
		{
			BWAPI::Broodwar->sendText("BM: Insufficient supply (%d/%d) for %s, building Supply Depot", 
				supplyOwned, type.supplyRequired(), type.c_str());	
			build(BWAPI::UnitTypes::Terran_Supply_Depot, true);
			return;
		}

		// Required units?
		map<BWAPI::UnitType, int> requiredUnits = type.requiredUnits();
		for (map<BWAPI::UnitType, int>::iterator i = requiredUnits.begin(); i != requiredUnits.end(); i++)
		{
			BWAPI::UnitType rt = (*i).first;
			int rc = (*i).second;
			if (BWAPI::Broodwar->self()->allUnitCount(rt) < rc)
			{
				BWAPI::Broodwar->sendText("!! Bad build order:  Need %dx%s to build %s", rc, rt.c_str(), type.c_str());
				return;
			}
		}
		
		// Required builder?
		BWAPI::UnitType builderType = type.whatBuilds().first;
		if (BWAPI::Broodwar->self()->allUnitCount(builderType) < 1)
		{
			BWAPI::Broodwar->sendText("!! Bad build order:  Need a %s to build %s", builderType.c_str(), type.c_str());
			return;
		}
		// OK - requirements met, let's build it!
		if (!type.isAddon())
		{
			// Find an idle builder
			for (set<Agent*>::iterator i = agents.begin(); i != agents.end(); i++)
			{
				if ((*i)->getUnit().getType().getID() == builderType.getID() &&
					(*i)->getState() == IdleState)
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
					BWAPI::Broodwar->sendText("BM: Worker found, building %s", type.c_str());
				}
			}
			// Wait if we didn't find a builder
			if (req.builder == NULL)
			{
				//BWAPI::Broodwar->sendText("BM: Could not find worker of type %s for %s", builderType.c_str(), 
				//	type.c_str());
				break;
			}
		}
		else
		{
			BWAPI::Broodwar->sendText("!! Bad build order: addons not yet implemented");
		}
		break;
	}

	case ISSUED:
	{
		//BWAPI::Broodwar->sendText("BM: ===== ISSUED =====");

		/* We need to determine if the order has been started (i.e. resources spent) */
		BWAPI::Unit* builder = req.builder;

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
			set<BWAPI::Unit*> ownedUnits = BWAPI::Broodwar->self()->getUnits();
			for (set<BWAPI::Unit*>::const_iterator i = ownedUnits.begin(); i != ownedUnits.end(); i++)
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

void BuildManager::build(BWAPI::UnitType type, bool immediate)
{
	BWAPI::TilePosition defaultLocation = BWAPI::Broodwar->self()->getStartLocation();
	build(type, defaultLocation, immediate);
}

void BuildManager::build(BWAPI::UnitType type, BWAPI::TilePosition goalPosition, bool immediate)
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
	BWAPI::Broodwar->sendText("BM: Rcvd order for %s", type.c_str());

	if (immediate) 
		buildStack.push(req);
	else
		buildQueue.push(req);
}
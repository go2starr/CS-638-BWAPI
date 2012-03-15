/*
 *  BuildManager  -  Used to produce new units, buildings and add-ons
 *
 *  Passes tasks down to the Construction or Production Managers
 */
#pragma once
#include "Manager.h"
#include "ConstructionManager.h"
#include "ProductionManager.h"
#include <queue>
#include <stack>

using std::queue;
using std::stack;

// States a req can be in
enum BuildReqState {
	NEW,
	ISSUED,
	STARTED,
	COMPLETE
};

// Information needed to complete a build request
typedef struct BuildReq {
	// External
	BWAPI::UnitType type;
	BWAPI::TilePosition goalPosition;
	int count;

	// Internal
	BuildReqState state;
	BWAPI::Unit *builder; // SCV or Structure
} BuildReq;

class BuildManager : public Manager
{
public: 
	void update();

	void build(BWAPI::UnitType type, bool immediate=false);
	void build(BWAPI::UnitType type, BWAPI::TilePosition goalPosition, bool immediate=false);

private:
	ConstructionManager constructionManager;
	ProductionManager   productionManager;

	queue<BuildReq> buildQueue;
	stack<BuildReq> currentReq;  // Requests can be recursive (dependencies)
};

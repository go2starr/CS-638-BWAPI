/*
 * BarracksAgent.cpp
 */
#include "BarracksAgent.h"
#include "StructureAgent.h"

#include <BWAPI.h>
#include <BWTA.h>

using namespace BWAPI;


BarracksAgent::BarracksAgent(Unit &u) 
	: StructureAgent(u)
{}

void BarracksAgent::update()
{
	// Send Units to nearest chokepoint
    BWTA::Chokepoint* chokepoint = BWTA::getNearestChokepoint(unit.getPosition());
    if( chokepoint != NULL )
        unit.setRallyPoint(chokepoint->getCenter());

	switch (state)
	{
	case TrainState:
        if (!unit.isTraining() && !unit.isBeingConstructed()) 
		{
			unit.train(UnitTypes::Terran_Marine);
		}
		break;
	}

	StructureAgent::update();
}

/*
 * BarracksAgent.cpp
 */
#include "UnitAgents/BarracksAgent.h"
#include "StructureAgent.h"

#include <BWAPI.h>

using namespace BWAPI;


BarracksAgent::BarracksAgent(Unit &u) 
	: StructureAgent(u)
{}

void BarracksAgent::update()
{
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

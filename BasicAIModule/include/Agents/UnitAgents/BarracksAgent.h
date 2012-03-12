/*
 *  BarracksAgent.h
 */
#pragma once
#include "StructureAgent.h"

#include <BWAPI.h>


class BarracksAgent : public StructureAgent
{
public:
	BarracksAgent(BWAPI::Unit& u);
	virtual void update();
};

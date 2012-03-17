/*
 *  CombatManager  -  Used to defend our base, and (eventually) attack other bases
 */
#pragma once
#include "Manager.h"

class CombatManager : public Manager
{
public: 
	void update();
    virtual const std::string getName() const { return "CombatMgr"; }
};

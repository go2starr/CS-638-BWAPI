/*
*  GasManager.h  - This manager sends it's workers to build refineries and gather gas 
*/
#pragma once
#include "Manager.h"

using std::vector;

class GasManager : public Manager
{



public:
	void update();
	int getGasRate();
	GasManager();

private:

	 int refineryCount;
	int refineryConstructingCount;
	int worksersConstructing;
	int newRefineries;
	
};


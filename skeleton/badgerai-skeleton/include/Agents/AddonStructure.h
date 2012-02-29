#pragma once
#include "Structure.h"


class AddonStructure : public Agent
{
private:
	Structure* associatedStructure;

public:
	AddonStructure(void);
	~AddonStructure(void);
};

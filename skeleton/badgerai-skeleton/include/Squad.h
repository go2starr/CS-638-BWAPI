#pragma once
#include "Actor.h"
#include <vector>

class Squad
{
private:
	std::vector<Actor*> units;

public:
	Squad(void);
	~Squad(void);
};

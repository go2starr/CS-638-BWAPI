#pragma once
#include "Agent.h"


class Actor : public Agent
{
private:
	Species species;

public:
	enum Species = { bio, mech };

	Actor(void);
	virtual ~Actor(void);
};

#pragma once
#include "Game.h"
#include "GameState.h"

class Hawk
{
private:
	Game* game;
	GameState* gameState;

public:
	Hawk(void);
	~Hawk(void);
};

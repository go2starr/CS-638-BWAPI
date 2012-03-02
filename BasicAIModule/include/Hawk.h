/* 
 * Hawk.h
 *
 * The Hawk is a singleton object whose responsibility is to examine 
 * the game's state as seen from BWAPI, and update the GameStateDB 
 * as needed to track changes.
 */
#pragma once
#include "BWAPI.h"
#include "GameStateDB.h"


class Hawk
{
private:

	GameStateDB& gameStateDB;

public:

	void update(BWAPI::Game* game);

	static Hawk& instance() { static Hawk instance; return instance; }

private:

	// Hawk is singleton, hence private ctors/assignment
	Hawk(GameStateDB& gameStateDB)
		: gameStateDB(gameStateDB) { }
	Hawk(const Hawk& hawk);
	void operator=(const Hawk& hawk);

};

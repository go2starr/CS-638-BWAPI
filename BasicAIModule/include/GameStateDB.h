/* 
 * GameStateDB.h
 *
 * The GameStateDB is a singleton object whose responsibility is to store 
 * the current state of the game and provide a querying mechanism for it.
 */
#pragma once
#include <map>
#include <string>

typedef std::map<GameStateEnum, std::string> GameState;

enum GameStateEnum
{
	TotalUnitCount, 
	WorkerCount, 
	FriendlyUnitCount, 
	EnemyUnitCount, 
	MineralsDiscovered, 
	GeysersDiscovered, 
	TO_BE_EXPANDED
};


class GameStateDB
{
private:
  GameState currentState, lastState;

public:
  Value& query(const GameStateEnum& key);	

private:
  
  // GameStateDB is singleton, hence private ctors/assignment
  GameStateDB();
  GameStateDB(const GameStateDB& other);
  void operator=(const GameStateDB& other);
};

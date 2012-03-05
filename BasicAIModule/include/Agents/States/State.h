/*
 *  State.h
 */
#pragma once
#include "Target.h"

class State
{
protected:
  Target target;               // Current target
  
public:
	// D'tor
	virtual ~State(){}
  
  // State entry
  virtual void onEnter();
  // State update
  virtual void onUpdate();
  // State exit
  virtual void onExit();

  // attr_accessor
  virtual Target& getTarget();

  // Enumeration
  int getId();
};

/*
 *  State.h
 */
#pragma once
#include "Target.h"

class State
{
protected:
  Target &target;               // Current target
  
public:
  // D'tor
  virtual ~State(){}
  
  // State entry
  virtual void onEnter()=0;
  // State update
  virtual void onUpdate()=0;
  // State exit
  virtual void onExit()=0;

  // attr_accessor
  virtual void setTarget(Target&);
  virtual Target& getTarget();

  // Enumeration
  int getId();
};

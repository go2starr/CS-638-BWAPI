/*
 *  State.h
 */
#pragma once
#include "../Agents/Agent.h"

class Agent;                    // Resolve circular dependency

template <class Target> 
class State
{
protected:
  Agent& agent;                 // Agent which is in our state
  Target& target;       // Current target
  
public:

  // C'tor / D'tor
  State(Agent &a, Target &t) :
    agent(a), target(t)
  ~State(){}
  
  // State entry
  virtual void onEnter()=0;
  // State update
  virtual void onUpdate()=0;
  // State exit
  virtual void onExit()=0;

  // attr_accessor
  void setTarget(Target&);
  Target& getTarget();

};

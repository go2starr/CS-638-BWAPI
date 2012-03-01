/*
 *  State.h
 */
#pragma once

template <class Target>
class State
{
 protected:
  Agent& agent;
  

 public:

  /* C'tor / D'tor */
  virtual State(){}
  virtual ~State(){}

  /* State entry */
  virtual void onEnter(Agent&, Target&)=0;

  /* State update */
  virtual void onUpdate(Agent&, Target&)=0;

  /* State exit */
  virtual void onExit(Agent&, Target&)=0;

};

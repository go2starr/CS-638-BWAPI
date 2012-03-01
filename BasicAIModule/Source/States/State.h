/*
 *  State.h
 */
#pragma once

template <class T>
class State
{
 public:

  /* C'tor / D'tor */
  virtual State(){}
  virtual ~State(){}

  /* State entry */
  virtual void onEnter(T&)=0;

  /* State update */
  virtual void onUpdate(T&)=0;

  /* State exit */
  virtual void onExit(T&)=0;

}

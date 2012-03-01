#pragma once

#include "../Agents/Agent.h"
#include "../States/State.h"

class Manager : Arbitrator::Controller<Agent*,double>
{

private:
  Arbitrator::Arbitrator<Agent*,double> &arbitrator;
  std::set<Agent> agents;
};

/*
 *  ResourceManager.h  - This manager sends it's workers to gather gas and minerals at maximum efficiency.
 */
#pragma once
#include "Manager.h"

class ResourceManager: public Manager
{
  int getGasRate();
  int getMineralRate();
};

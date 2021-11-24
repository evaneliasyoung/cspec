/**
 *  @file      ns.h
 *  @brief     Namspace extensions for cspec memory.
 *
 *  @author    Evan Elias Young
 *  @date      2021-11-23
 *  @date      2021-11-23
 *  @copyright Copyright 2021 Evan Elias Young. All rights reserved.
 */

#pragma once

#include "../core.h"

#include <vector>

namespace cspec
{
  namespace memory
  {
    struct voltages_t
    {
      double min;
      double max;
      double configured;
    };

    struct memory_t
    {
      voltages_t voltage;
      string form_factor;
      umax size;
      umax speed;
      string manufacturer;
      string model;
      string serial;
      string bank;
    };

    vector<memory_t> devices();
  } // namespace memory
} // namespace cspec

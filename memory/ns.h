/**
 *  @file      ns.h
 *  @brief     Namspace extensions for cspec memory.
 *
 *  @author    Evan Elias Young
 *  @date      2021-11-23
 *  @date      2021-11-24
 *  @copyright Copyright 2021 Evan Elias Young. All rights reserved.
 */

#pragma once

#include "../core.h"

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
    void to_json(json &j, const voltages_t &vlt);
    void from_json(const json &j, voltages_t &vlt);

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
    void to_json(json &j, const memory_t &mem);
    void from_json(const json &j, memory_t &mem);

    vector<memory_t> devices();
    json collect();
    json collect(const vector<string> &keys);
  } // namespace memory
} // namespace cspec

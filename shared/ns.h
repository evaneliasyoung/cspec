/**
 *  @file      ns.h
 *  @brief     Shared structures and methods in cspec.
 *
 *  @author    Evan Elias Young
 *  @date      2021-11-24
 *  @date      2021-11-24
 *  @copyright Copyright 2021 Evan Elias Young. All rights reserved.
 */

#pragma once

#include "../core.h"

#include <sstream>

namespace cspec
{
  namespace shared
  {
    struct version_t
    {
      u32 major;
      u32 minor;
      u32 patch;
      u32 build;
    };
    string vertos(const version_t &ver);
    version_t stover(const string &ver);
    void to_json(json &j, const version_t &ver);
    void from_json(const json &j, version_t &ver);
  } // namespace shared
} // namespace cspec

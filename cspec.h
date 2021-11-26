/**
 *  @file      cspec.h
 *  @brief     Namespace definitions for cspec.
 *
 *  @author    Evan Elias Young
 *  @date      2021-11-11
 *  @date      2021-11-25
 *  @copyright Copyright 2021 Evan Elias Young. All rights reserved.
 */

#pragma once

#include "core.h"
#include "cpu/ns.h"
#include "filesystem/ns.h"
#include "gpu/ns.h"
#include "memory/ns.h"
#include "shared/ns.h"
#include "system/ns.h"

namespace cspec
{
  json collect(const std::map<string, vector<string>> &captured);
} // namespace cspec

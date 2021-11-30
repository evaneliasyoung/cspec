/**
 *  @file      strconv.cpp
 *  @brief     Handles cpu related string conversions.
 *
 *  @author    Evan Elias Young
 *  @date      2021-11-29
 *  @date      2021-11-29
 *  @copyright Copyright 2021 Evan Elias Young. All rights reserved.
 */

#include "../shared/ns.h"
#include "ns.h"

cspec::cpu::architecture_t cspec::cpu::stoarch(const string &architecture)
{
  if (cspec::shared::icaseis(architecture, "x86_64"))
    return cspec::cpu::architecture_t::x64;
  else if (cspec::shared::icaseis(architecture, "i686"))
    return cspec::cpu::architecture_t::x86;
  else if (strstr(architecture.c_str(), "arm") == architecture.c_str())
    return cspec::cpu::architecture_t::arm;
  else if (cspec::shared::icaseis(architecture, "ia64"))
    return cspec::cpu::architecture_t::itanium;
  else
    return cspec::cpu::architecture_t::unknown;
}

string cspec::cpu::archtos(const cspec::cpu::architecture_t &architecture)
{
  switch (architecture)
  {
    case cspec::cpu::architecture_t::x64:
      return "x64";
    case cspec::cpu::architecture_t::x86:
      return "x86";
    case cspec::cpu::architecture_t::arm:
      return "ARM";
    case cspec::cpu::architecture_t::itanium:
      return "Itanium";
    default:
      return "Unknown";
  }
}

cspec::cpu::cache_type_t cspec::cpu::stocch(const string &cache_type)
{
  if (cspec::shared::icaseis(cache_type, "Unified"))
    return cspec::cpu::cache_type_t::instruction;
  if (cspec::shared::icaseis(cache_type, "Instruction"))
    return cspec::cpu::cache_type_t::instruction;
  if (cspec::shared::icaseis(cache_type, "Data"))
    return cspec::cpu::cache_type_t::data;
  if (cspec::shared::icaseis(cache_type, "Trace"))
    return cspec::cpu::cache_type_t::trace;
  return cspec::cpu::cache_type_t::unknown;
}

string cspec::cpu::cchtos(const cspec::cpu::cache_type_t &cache_type)
{
  switch (cache_type)
  {
    case cspec::cpu::cache_type_t::unified:
      return "unified";
    case cspec::cpu::cache_type_t::instruction:
      return "instruction";
    case cspec::cpu::cache_type_t::data:
      return "data";
    case cspec::cpu::cache_type_t::trace:
      return "trace";
    case cspec::cpu::cache_type_t::unknown:
      [[fallthrough]];
    default:
      return "unknown";
  }
}

cspec::cpu::endian_t cspec::cpu::stoend(const string &endian_type)
{
  if (cspec::shared::icaseis(endian_type, "Big Endian"))
    return cspec::cpu::endian_t::big;
  return cspec::cpu::endian_t::little;
}

string cspec::cpu::endtos(const cspec::cpu::endian_t &endian_type)
{
  switch (endian_type)
  {
    case cspec::cpu::endian_t::big:
      return "big";
    case cspec::cpu::endian_t::little:
      return "little";
    default:
      return "unknown";
  }
}

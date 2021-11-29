/**
 *  @file      cache.cpp
 *  @brief     Determines cpu cache level info.
 *
 *  @author    Evan Elias Young
 *  @date      2021-11-16
 *  @date      2021-11-29
 *  @copyright Copyright 2021 Evan Elias Young. All rights reserved.
 */

#include "../utils/strcmp.hpp"
#include "ns.h"

cspec::cpu::cache_type_t cspec::cpu::stocch(const string &cache_type)
{
  if (icaseis(cache_type, "Unified"))
    return cspec::cpu::cache_type_t::instruction;
  if (icaseis(cache_type, "Instruction"))
    return cspec::cpu::cache_type_t::instruction;
  if (icaseis(cache_type, "Data"))
    return cspec::cpu::cache_type_t::data;
  if (icaseis(cache_type, "Trace"))
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

void cspec::cpu::to_json(json &j, const cspec::cpu::cache_t &cch)
{
  j = json{{"association", cch.association},
           {"line_size", cch.line_size},
           {"size", cch.size},
           {"type", cspec::cpu::cchtos(cch.type)}};
}

void cspec::cpu::from_json(const json &j, cspec::cpu::cache_t &cch)
{
  j.at("association").get_to(cch.association);
  j.at("line_size").get_to(cch.line_size);
  j.at("size").get_to(cch.size);
  cch.type = cspec::cpu::stocch(j.at("type"));
}

#if defined(WIN)
#include "../utils/win/cpuinfo.hpp"

cspec::cpu::cache_t cspec::cpu::cache(u8 level)
{
  for (auto &&info: cpuinfo_buffer())
  {
    if (info.Relationship == RelationCache)
    {
      if (info.Cache.Level == level)
      {
        cspec::cpu::cache_type_t type{};
        switch (info.Cache.Type)
        {
          case CacheUnified:
            type = cspec::cpu::cache_type_t::unified;
            break;
          case CacheInstruction:
            type = cspec::cpu::cache_type_t::instruction;
            break;
          case CacheData:
            type = cspec::cpu::cache_type_t::data;
            break;
          case CacheTrace:
            type = cspec::cpu::cache_type_t::trace;
            break;
        }
        return {info.Cache.Size, info.Cache.LineSize, info.Cache.Associativity, type};
      }
    }
  }

  return {};
}
#elif defined(MAC)
cspec::cpu::cache_t cspec::cpu::cache(u8 level)
{
  cspec::cpu::cache_t ret{};

  return ret;
}
#else
#include "../utils/bitpow.hpp"

#include <fstream>

cspec::cpu::cache_t cspec::cpu::cache(u8 level)
{
  string prefix("/sys/devices/system/cpu/cpu0/cache/index" + std::to_string(level) + '/');
  cspec::cpu::cache_t ret{};

  {
    std::ifstream size(prefix + "size");
    if (size.is_open() && size)
    {
      u8 suffix;
      size >> ret.size >> suffix;
      switch (suffix)
      {
        case 'G':
          ret.size *= 1024;
          [[fallthrough]];
        case 'M':
          ret.size *= 1024;
          [[fallthrough]];
        case 'K':
          ret.size *= 1024;
      }
      ret.size = depow2(ret.size);
    }
  }

  {
    std::ifstream line_size(prefix + "coherency_line_size");
    if (line_size.is_open() && line_size)
      line_size >> ret.line_size;
  }

  {
    std::ifstream association(prefix + "ways_of_associativity");
    if (association.is_open() && association)
    {
      u32 tmp;
      association >> tmp;
      ret.association = tmp;
    }
  }

  {
    std::ifstream type(prefix + "type");
    if (type.is_open() && type)
    {
      string tmp;
      type >> tmp;
      if (tmp.find("nified") == 1)
        ret.type = cspec::cpu::cache_type_t::unified;
      else if (tmp.find("nstruction") == 1)
        ret.type = cspec::cpu::cache_type_t::instruction;
      else if (tmp.find("ata") == 1)
        ret.type = cspec::cpu::cache_type_t::data;
      else if (tmp.find("race") == 1)
        ret.type = cspec::cpu::cache_type_t::trace;
    }
  }

  return ret;
}
#endif

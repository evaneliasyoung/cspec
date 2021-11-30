/**
 *  @file      cache.cpp
 *  @brief     Determines cpu cache level info.
 *
 *  @author    Evan Elias Young
 *  @date      2021-11-16
 *  @date      2021-11-29
 *  @copyright Copyright 2021 Evan Elias Young. All rights reserved.
 */

#include "../shared/ns.h"
#include "ns.h"

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

#if defined(WIN)
cspec::cpu::cache_t cspec::cpu::cache(u8 level)
{
  for (auto &&info: cspec::shared::cpuinfo_buffer())
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
  array<u64, 10> all_buf;

  cspec::shared::sysctlintegral("hw.cachelinesize", ret.line_size);
  if (cspec::shared::sysctlarray("hw.cachesize", all_buf))
    ret.size = cspec::shared::depow2(all_buf[level == 0 ? 1 : level]);
  if (cspec::shared::sysctlarray("hw.cacheconfig", all_buf))
    ret.association = cspec::shared::depow2(all_buf[level]);
  ret.type = cspec::cpu::cache_type_t::unified;

  return ret;
}
#else
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
      ret.size = cspec::shared::depow2(ret.size);
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

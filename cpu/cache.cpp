/**
 *  @file      cache.cpp
 *  @brief     Determines cpu cache level info.
 *
 *  @author    Evan Elias Young
 *  @date      2021-11-16
 *  @date      2021-11-16
 *  @copyright Copyright 2021 Evan Elias Young. All rights reserved.
 */

#include "ns.h"
#include "utils.hpp"

#if defined(WINDOWS)
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
#else
cspec::cpu::cache_t cspec::cpu::cache(u8 level)
{
  std::string prefix("/sys/devices/system/cpu/cpu0/cache/index" + std::to_string(level) + '/');
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
      std::string tmp;
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

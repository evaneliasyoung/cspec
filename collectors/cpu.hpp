/**
 *  @file      cpu.hpp
 *  @brief     Collects cpu information to a json object.
 *
 *  @author    Evan Elias Young
 *  @date      2021-11-24
 *  @date      2021-11-24
 *  @copyright Copyright 2021 Evan Elias Young. All rights reserved.
 */

#pragma once

#include "../core.h"
#include "../cpu/ns.h"

json collect_cpu()
{
  auto ret = R"({})"_json;

  ret["name"] = cspec::cpu::name();
  ret["amounts"] = cspec::cpu::amounts();
  ret["architecture"] = cspec::cpu::archtos(cspec::cpu::architecture());
  ret["clock"] = cspec::cpu::clock();
  ret["endian"] = cspec::cpu::endian() == cspec::cpu::endian_t::big ? "big" : "little";
  ret["vendor"] = cspec::cpu::vendor();
  ret["group"] = cspec::cpu::group();
  {
    auto caches = array<cspec::cpu::cache_t, 4>{};
    for (auto i = 0; i < 4; ++i)
      caches[i] = cspec::cpu::cache(i);
    ret["cache"] = json(caches);
  }

  return ret;
}

json collect_cpu(const vector<string> &keys)
{
  if (keys.size() == 1 && (strcasecmp(keys[0].c_str(), "all") == 0 || keys[0] == "*"))
    return collect_cpu();

  auto ret = R"({})"_json;
  for (const auto &key: keys)
  {
    if (strcasecmp(key.c_str(), "name") == 0)
      ret["name"] = cspec::cpu::name();
    else if (strcasecmp(key.c_str(), "amounts") == 0)
      ret["amounts"] = cspec::cpu::amounts();
    else if (strcasecmp(key.c_str(), "architecture") == 0)
      ret["architecture"] = cspec::cpu::archtos(cspec::cpu::architecture());
    else if (strcasecmp(key.c_str(), "clock") == 0)
      ret["clock"] = cspec::cpu::clock();
    else if (strcasecmp(key.c_str(), "endian") == 0)
      ret["endian"] = cspec::cpu::endian() == cspec::cpu::endian_t::big ? "big" : "little";
    else if (strcasecmp(key.c_str(), "vendor") == 0)
      ret["vendor"] = cspec::cpu::vendor();
    else if (strcasecmp(key.c_str(), "group") == 0)
      ret["group"] = cspec::cpu::group();
    else if (strcasecmp(key.c_str(), "cache") == 0)
    {
      auto caches = array<cspec::cpu::cache_t, 4>{};
      for (auto i = 0; i < 4; ++i)
        caches[i] = cspec::cpu::cache(i);
      ret["cache"] = json(caches);
    }
    else
    {
      throw std::invalid_argument("invalid CPU request key.");
    }
  }

  return ret;
}

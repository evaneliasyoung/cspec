/**
 *  @file      cpu.cpp
 *  @brief     Collects cpu information to a json object.
 *
 *  @author    Evan Elias Young
 *  @date      2021-11-24
 *  @date      2021-11-25
 *  @copyright Copyright 2021 Evan Elias Young. All rights reserved.
 */

#include "../utils/strcmp.hpp"
#include "ns.h"

json cspec::cpu::collect()
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

json cspec::cpu::collect(const vector<string> &keys)
{
  if (keys.size() == 1 && (icaseis(keys[0], "all") || keys[0] == "*"))
    return cspec::cpu::collect();

  auto ret = R"({})"_json;
  for (const auto &key: keys)
  {
    if (icaseis(key, "name"))
      ret["name"] = cspec::cpu::name();
    else if (icaseis(key, "amounts"))
      ret["amounts"] = cspec::cpu::amounts();
    else if (icaseis(key, "architecture"))
      ret["architecture"] = cspec::cpu::archtos(cspec::cpu::architecture());
    else if (icaseis(key, "clock"))
      ret["clock"] = cspec::cpu::clock();
    else if (icaseis(key, "endian"))
      ret["endian"] = cspec::cpu::endian() == cspec::cpu::endian_t::big ? "big" : "little";
    else if (icaseis(key, "vendor"))
      ret["vendor"] = cspec::cpu::vendor();
    else if (icaseis(key, "group"))
      ret["group"] = cspec::cpu::group();
    else if (icaseis(key, "cache"))
    {
      auto caches = array<cspec::cpu::cache_t, 4>{};
      for (auto i = 0; i < 4; ++i)
        caches[i] = cspec::cpu::cache(i);
      ret["cache"] = json(caches);
    }
    else
      throw std::invalid_argument("invalid CPU request key.");
  }

  return ret;
}

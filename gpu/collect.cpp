/**
 *  @file      collect.cpp
 *  @brief     Collects gpu information to a json object.
 *
 *  @author    Evan Elias Young
 *  @date      2021-11-24
 *  @date      2021-11-25
 *  @copyright Copyright 2021 Evan Elias Young. All rights reserved.
 */

#include "../utils/strcmp.hpp"
#include "ns.h"

json cspec::gpu::collect()
{
  auto ret = R"([])"_json;
  const auto devices = cspec::gpu::devices();

  for (const auto gpu: devices)
    ret.push_back(gpu);

  return ret;
}

json cspec::gpu::collect(const vector<string> &keys)
{
  if (keys.size() == 1 && (icaseis(keys[0], "all") || keys[0] == "*"))
    return cspec::gpu::collect();

  auto ret = R"([])"_json;
  const auto devices = cspec::gpu::devices();
  for (const auto &gpu: devices)
  {
    auto gpuj = R"({})"_json;
    for (const auto &key: keys)
    {
      if (icaseis(key, "name"))
        gpuj["name"] = gpu.name;
      else if (icaseis(key, "memory"))
        gpuj["memory"] = gpu.memory;
      else if (icaseis(key, "bus"))
        gpuj["bus"] = gpu.bus;
      else if (icaseis(key, "dynamic"))
        gpuj["dynamic"] = gpu.dynamic;
      else if (icaseis(key, "vendor"))
        gpuj["vendor"] = cspec::gpu::vndtos(gpu.vendor);
    }
    ret.push_back(gpuj);
  }

  return ret;
}

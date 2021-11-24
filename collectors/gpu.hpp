/**
 *  @file      gpu.hpp
 *  @brief     Collects gpu information to a json object.
 *
 *  @author    Evan Elias Young
 *  @date      2021-11-24
 *  @date      2021-11-24
 *  @copyright Copyright 2021 Evan Elias Young. All rights reserved.
 */

#pragma once

#include "../core.h"
#include "../gpu/ns.h"

json collect_gpu()
{
  auto ret = R"([])"_json;
  const auto devices = cspec::gpu::devices();

  for (const auto gpu: devices)
    ret.push_back(gpu);

  return ret;
}

json collect_gpu(const vector<string> &keys)
{
  if (keys.size() == 1 && (strcasecmp(keys[0].c_str(), "all") == 0 || keys[0] == "*"))
    return collect_gpu();

  auto ret = R"([])"_json;
  const auto devices = cspec::gpu::devices();
  for (const auto &gpu: devices)
  {
    auto gpuj = R"({})"_json;
    for (const auto &key: keys)
    {
      if (strcasecmp(key.c_str(), "name") == 0)
        gpuj["name"] = gpu.name;
      else if (strcasecmp(key.c_str(), "memory") == 0)
        gpuj["memory"] = gpu.memory;
      else if (strcasecmp(key.c_str(), "vendor") == 0)
        gpuj["architecture"] = cspec::gpu::vndtos(gpu.vendor);
      else
      {
        throw std::invalid_argument("invalid gpu request key.");
      }
    }
    ret.push_back(gpuj);
  }

  return ret;
}

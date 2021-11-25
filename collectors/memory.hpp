/**
 *  @file      memory.hpp
 *  @brief     Collects memory information to a json object.
 *
 *  @author    Evan Elias Young
 *  @date      2021-11-24
 *  @date      2021-11-24
 *  @copyright Copyright 2021 Evan Elias Young. All rights reserved.
 */

#pragma once

#include "../core.h"
#include "../memory/ns.h"

json collect_memory()
{
  auto ret = R"([])"_json;
  const auto devices = cspec::memory::devices();

  for (const auto memory: devices)
    ret.push_back(memory);

  return ret;
}

json collect_memory(const vector<string> &keys)
{
  if (keys.size() == 1 && (strcasecmp(keys[0].c_str(), "all") == 0 || keys[0] == "*"))
    return collect_memory();

  auto ret = R"([])"_json;
  const auto devices = cspec::memory::devices();
  for (const auto &memory: devices)
  {
    auto memoryj = R"({})"_json;
    for (const auto &key: keys)
    {
      if (strcasecmp(key.c_str(), "voltage") == 0)
        memoryj["voltage"] = memory.voltage;
      else if (strcasecmp(key.c_str(), "form_factor") == 0)
        memoryj["form_factor"] = memory.form_factor;
      else if (strcasecmp(key.c_str(), "size") == 0)
        memoryj["size"] = memory.size;
      else if (strcasecmp(key.c_str(), "speed") == 0)
        memoryj["speed"] = memory.speed;
      else if (strcasecmp(key.c_str(), "manufacturer") == 0)
        memoryj["manufacturer"] = memory.manufacturer;
      else if (strcasecmp(key.c_str(), "model") == 0)
        memoryj["model"] = memory.model;
      else if (strcasecmp(key.c_str(), "serial") == 0)
        memoryj["serial"] = memory.serial;
      else if (strcasecmp(key.c_str(), "bank") == 0)
        memoryj["bank"] = memory.bank;
      else
      {
        throw std::invalid_argument("invalid memory request key.");
      }
    }
    ret.push_back(memoryj);
  }

  return ret;
}

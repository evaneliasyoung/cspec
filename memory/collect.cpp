/**
 *  @file      collect.cpp
 *  @brief     Collects memory information to a json object.
 *
 *  @author    Evan Elias Young
 *  @date      2021-11-24
 *  @date      2021-11-25
 *  @copyright Copyright 2021 Evan Elias Young. All rights reserved.
 */

#include "../utils/strcmp.hpp"
#include "ns.h"

json cspec::memory::collect()
{
  auto ret = R"([])"_json;
  const auto devices = cspec::memory::devices();

  for (const auto memory: devices)
    ret.push_back(memory);

  return ret;
}

json cspec::memory::collect(const vector<string> &keys)
{
  if (keys.size() == 1 && (icaseis(keys[0], "all") || keys[0] == "*"))
    return cspec::memory::collect();

  auto ret = R"([])"_json;
  const auto devices = cspec::memory::devices();
  for (const auto &memory: devices)
  {
    auto memoryj = R"({})"_json;
    for (const auto &key: keys)
    {
      if (icaseis(key, "voltage"))
        memoryj["voltage"] = memory.voltage;
      else if (icaseis(key, "form_factor"))
        memoryj["form_factor"] = memory.form_factor;
      else if (icaseis(key, "size"))
        memoryj["size"] = memory.size;
      else if (icaseis(key, "speed"))
        memoryj["speed"] = memory.speed;
      else if (icaseis(key, "manufacturer"))
        memoryj["manufacturer"] = memory.manufacturer;
      else if (icaseis(key, "model"))
        memoryj["model"] = memory.model;
      else if (icaseis(key, "serial"))
        memoryj["serial"] = memory.serial;
      else if (icaseis(key, "bank"))
        memoryj["bank"] = memory.bank;
    }
    ret.push_back(memoryj);
  }

  return ret;
}

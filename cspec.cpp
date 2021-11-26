/**
 *  @file      cspec.cpp
 *  @brief     Namespace methods.
 *
 *  @author    Evan Elias Young
 *  @date      2021-11-25
 *  @date      2021-11-25
 *  @copyright Copyright 2021 Evan Elias Young. All rights reserved.
 */

#include "cspec.h"

#include <iostream>

json cspec::collect(const std::map<string, vector<string>> &captured)
{
  auto j = R"({})"_json;

  for (const auto &[ns, keys]: captured)
  {
    if (ns == "cpu")
      j["cpu"] = cspec::cpu::collect(keys);
    else if (ns == "filesystem")
      j["filesystem"] = cspec::filesystem::collect(keys);
    else if (ns == "gpu")
      j["gpu"] = cspec::gpu::collect(keys);
    else if (ns == "memory")
      j["memory"] = cspec::memory::collect(keys);
    else if (ns == "system")
      j["system"] = cspec::system::collect(keys);
  }
  return j;
}

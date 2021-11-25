/**
 *  @file      collect.cpp
 *  @brief     Collects system information to a json object.
 *
 *  @author    Evan Elias Young
 *  @date      2021-11-24
 *  @date      2021-11-25
 *  @copyright Copyright 2021 Evan Elias Young. All rights reserved.
 */

#include "../utils/strcmp.hpp"
#include "ns.h"

json cspec::system::collect()
{
  auto ret = R"({})"_json;

  ret["os"] = cspec::system::os();
  ret["kernel"] = cspec::system::kernel();
  ret["times"] = cspec::system::times();

  return ret;
}

json cspec::system::collect(const vector<string> &keys)
{
  if (keys.size() == 1 && (icaseis(keys[0], "all") || keys[0] == "*"))
    return cspec::system::collect();

  auto ret = R"({})"_json;
  for (const auto &key: keys)
  {
    if (icaseis(key, "os"))
      ret["os"] = cspec::system::os();
    else if (icaseis(key, "kernel"))
      ret["kernel"] = cspec::system::kernel();
    else if (icaseis(key, "times"))
      ret["times"] = cspec::system::times();
    else
      throw std::invalid_argument("invalid system request key.");
  }
  return ret;
}

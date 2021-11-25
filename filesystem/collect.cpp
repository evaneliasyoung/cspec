/**
 *  @file      collect.cpp
 *  @brief     Collects filesystem information to a json object.
 *
 *  @author    Evan Elias Young
 *  @date      2021-11-24
 *  @date      2021-11-25
 *  @copyright Copyright 2021 Evan Elias Young. All rights reserved.
 */

#include "../utils/strcmp.hpp"
#include "ns.h"

json cspec::filesystem::collect()
{
  auto ret = R"([])"_json;
  const auto systems = cspec::filesystem::systems();

  for (const auto filesystem: systems)
    ret.push_back(filesystem);

  return ret;
}

json cspec::filesystem::collect(const vector<string> &keys)
{
  if (keys.size() == 1 && (icaseis(keys[0], "all") || keys[0] == "*"))
    return cspec::filesystem::collect();

  auto ret = R"([])"_json;
  const auto systems = cspec::filesystem::systems();
  for (const auto &filesystem: systems)
  {
    auto filesystemj = R"({})"_json;
    for (const auto &key: keys)
    {
      if (icaseis(key, "name"))
        filesystemj["name"] = filesystem.name;
      else if (icaseis(key, "sizes"))
        filesystemj["sizes"] = filesystem.sizes;
      else if (icaseis(key, "mount"))
        filesystemj["mount"] = filesystem.mount;
      else if (icaseis(key, "type"))
        filesystemj["architecture"] = cspec::filesystem::fstos(filesystem.type);
      else
        throw std::invalid_argument("invalid filesystem request key.");
    }
    ret.push_back(filesystemj);
  }

  return ret;
}

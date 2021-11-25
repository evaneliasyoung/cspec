/**
 *  @file      collect.cpp
 *  @brief     Collects filesystem information to a json object.
 *
 *  @author    Evan Elias Young
 *  @date      2021-11-24
 *  @date      2021-11-25
 *  @copyright Copyright 2021 Evan Elias Young. All rights reserved.
 */

#include "../core.h"
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
  if (keys.size() == 1 && (strcasecmp(keys[0].c_str(), "all") == 0 || keys[0] == "*"))
    return cspec::filesystem::collect();

  auto ret = R"([])"_json;
  const auto systems = cspec::filesystem::systems();
  for (const auto &filesystem: systems)
  {
    auto filesystemj = R"({})"_json;
    for (const auto &key: keys)
    {
      if (strcasecmp(key.c_str(), "name") == 0)
        filesystemj["name"] = filesystem.name;
      else if (strcasecmp(key.c_str(), "sizes") == 0)
        filesystemj["sizes"] = filesystem.sizes;
      else if (strcasecmp(key.c_str(), "mount") == 0)
        filesystemj["mount"] = filesystem.mount;
      else if (strcasecmp(key.c_str(), "type") == 0)
        filesystemj["architecture"] = cspec::filesystem::fstos(filesystem.type);
      else
        throw std::invalid_argument("invalid filesystem request key.");
    }
    ret.push_back(filesystemj);
  }

  return ret;
}
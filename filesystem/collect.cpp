/**
 *  @file      collect.cpp
 *  @brief     Collects filesystem information to a json object.
 *
 *  @author    Evan Elias Young
 *  @date      2021-11-24
 *  @date      2021-11-29
 *  @copyright Copyright 2021 Evan Elias Young. All rights reserved.
 */

#include "../shared/ns.h"

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
  if (keys.size() == 1 && (cspec::shared::icaseis(keys[0], "all") || keys[0] == "*"))
    return cspec::filesystem::collect();

  auto ret = R"([])"_json;
  const auto systems = cspec::filesystem::systems();
  for (const auto &filesystem: systems)
  {
    auto filesystemj = R"({})"_json;
    for (const auto &key: keys)
    {
      if (cspec::shared::icaseis(key, "name"))
        filesystemj["name"] = filesystem.name;
      else if (cspec::shared::icaseis(key, "sizes"))
        filesystemj["sizes"] = filesystem.sizes;
      else if (cspec::shared::icaseis(key, "mount"))
        filesystemj["mount"] = filesystem.mount;
      else if (cspec::shared::icaseis(key, "type"))
        filesystemj["type"] = cspec::filesystem::fstos(filesystem.type);
    }
    ret.push_back(filesystemj);
  }

  return ret;
}

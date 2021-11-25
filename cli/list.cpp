/**
 *  @file      list.cpp
 *  @brief     List available queries in all or select namespaces.
 *
 *  @author    Evan Elias Young
 *  @date      2021-11-25
 *  @date      2021-11-25
 *  @copyright Copyright 2021 Evan Elias Young. All rights reserved.
 */

#include "ns.h"

#include <map>

void cspec::cli::list(argparse::ArgumentParser args)
{
  auto listings = args.get<vector<string>>("query");
  auto format = args.get<cspec::cli::format_t>("format");
  const std::map<string, vector<string>> all_queries = {
    {"cpu", {"all", "name", "amounts", "architecture", "clock", "endian", "vendor", "group"}},
    {"filesystem", {"all", "name", "sizes", "mount", "type"}},
    {"gpu", {"all", "name", "memory", "architecture"}},
    {"memory", {"all", "voltage", "form_factor", "size", "speed", "manufacturer", "model", "serial", "bank"}},
    {"system", {"all", "os", "kernel", "times"}}};
  const vector<string> namespaces = {"cpu", "filesystem", "gpu", "memory", "system"};
  std::map<string, vector<string>> captured = {};
  bool overview = false;
  bool complete = false;

  for (const auto &ns: listings)
  {
    if (strcasecmp(ns.c_str(), "all.all") == 0)
    {
      complete = true;
      break;
    }
    else if (strcasecmp(ns.c_str(), "all") == 0)
    {
      overview = true;
      break;
    }
    else if (strcasecmp(ns.c_str(), "CPU") == 0)
      captured.insert({"cpu", all_queries.at("cpu")});
    else if (strcasecmp(ns.c_str(), "Filesystem") == 0)
      captured.insert({"filesystem", all_queries.at("filesystem")});
    else if (strcasecmp(ns.c_str(), "GPU") == 0)
      captured.insert({"gpu", all_queries.at("gpu")});
    else if (strcasecmp(ns.c_str(), "Memory") == 0)
      captured.insert({"memory", all_queries.at("memory")});
    else if (strcasecmp(ns.c_str(), "System") == 0)
      captured.insert({"system", all_queries.at("system")});
  }

  if (format == cspec::cli::format_t::json)
    if (complete)
      std::cout << json(all_queries).dump(2) << '\n';
    else if (overview)
      std::cout << json(namespaces).dump(2) << '\n';
    else
      std::cout << json(captured).dump(2) << '\n';
  else if (complete)
    for (const auto &[ns, keys]: all_queries)
      for (const auto &key: keys)
        std::cout << ns << '.' << key << '\n';
  else if (overview)
    for (const auto &[ns, keys]: all_queries)
      std::cout << ns << '\n';
  else
    for (const auto &[ns, keys]: captured)
      for (const auto &key: keys)
        std::cout << ns << '.' << key << '\n';
}

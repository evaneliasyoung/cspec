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

void cspec::cli::list(argparse::ArgumentParser args)
{
  auto listings = args.get<vector<string>>("query");
  auto format = args.get<cspec::cli::format_t>("format");
  std::map<string, vector<string>> captured = {};
  bool overview = false;
  bool complete = false;

  for (const auto &ns: listings)
  {
    if (icaseis(ns, "all.all"))
    {
      complete = true;
      break;
    }
    else if (icaseis(ns, "all"))
    {
      overview = true;
      break;
    }
    else
      captured.insert({ns, cspec::cli::queries.at(ns)});
  }

  if (format == cspec::cli::format_t::json)
    if (complete)
      std::cout << json(cspec::cli::queries).dump(2) << '\n';
    else if (overview)
      std::cout << json(cspec::cli::namespaces).dump(2) << '\n';
    else
      std::cout << json(captured).dump(2) << '\n';
  else if (complete)
    for (const auto &[ns, keys]: cspec::cli::queries)
      for (const auto &key: keys)
        std::cout << ns << '.' << key << '\n';
  else if (overview)
    for (const auto &[ns, keys]: cspec::cli::queries)
      std::cout << ns << '\n';
  else
    for (const auto &[ns, keys]: captured)
      for (const auto &key: keys)
        std::cout << ns << '.' << key << '\n';
}

/**
 *  @file      get.cpp
 *  @brief     Make queries in all or select namespaces.
 *
 *  @author    Evan Elias Young
 *  @date      2021-11-25
 *  @date      2021-11-25
 *  @copyright Copyright 2021 Evan Elias Young. All rights reserved.
 */

#include "ns.h"

void cspec::cli::get(argparse::ArgumentParser args)
{
  auto listings = args.get<vector<string>>("query");
  const auto format = args.get<cspec::cli::format_t>("format");
  std::sort(listings.begin(), listings.end());
  std::map<string, vector<string>> captured = {};
  bool complete = false;

  for (auto key: listings)
  {
    to_lower(key);
    const auto per_idx = key.find_first_of('.');
    const auto ns = per_idx == string::npos ? key : key.substr(0, per_idx);
    const auto query = per_idx == string::npos ? "all" : key.substr(per_idx + 1);

    if (icaseis(ns, "all"))
    {
      complete = true;
      break;
    }
    else if (contains_icase(cspec::cli::namespaces, ns))
    {
      if (!contains_icase(cspec::cli::queries.at(ns), query))
        throw cspec::cli::invalid_query(ns, query);
      std::cout << ns << " -> " << query << '\n';
    }
    else
      throw cspec::cli::invalid_namespace(ns);
  }
}

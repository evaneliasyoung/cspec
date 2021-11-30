/**
 *  @file      list.cpp
 *  @brief     List available queries in all or select namespaces.
 *
 *  @author    Evan Elias Young
 *  @date      2021-11-25
 *  @date      2021-11-29
 *  @copyright Copyright 2021 Evan Elias Young. All rights reserved.
 */

#include "ns.h"

void cspec::cli::list(argparse::ArgumentParser args)
{
  const auto format = args.get<cspec::cli::format_t>("format");
  const auto query = args.get<vector<string>>("query");

  if (cspec::shared::contains_icase(query, "all.all"))
    stream(format, cspec::cli::queries);
  else if (cspec::shared::contains_icase(query, "all"))
    stream(format, cspec::cli::namespaces);
  else
  {
    std::map<string, vector<string>> collected = {};
    for (const auto &ns: query)
      if (cspec::shared::contains_icase(cspec::cli::namespaces, ns))
        collected.insert({ns, cspec::cli::queries.at(ns)});
      else
        throw cspec::cli::invalid_namespace(ns);
    stream(format, collected);
  }
}

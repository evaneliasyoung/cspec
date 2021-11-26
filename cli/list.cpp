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

void stream(const cspec::cli::format_t &format, const std::map<string, vector<string>> &queries)
{
  if (format == cspec::cli::format_t::json)
    std::cout << json(queries).dump(2) << '\n';
  else
    for (const auto &[ns, keys]: queries)
      for (const auto &key: keys)
        std::cout << ns << '.' << key << '\n';
}

void stream(const cspec::cli::format_t &format, const vector<string> &namespaces)
{
  if (format == cspec::cli::format_t::json)
    std::cout << json(namespaces).dump(2) << '\n';
  else
    for (const auto &ns: namespaces)
      std::cout << ns << '\n';
}

std::map<string, vector<string>> capture(const vector<string> &query)
{
  std::map<string, vector<string>> captured = {};
  for (const auto &ns: query)
    if (contains_icase(cspec::cli::namespaces, ns))
      captured.insert({ns, cspec::cli::queries.at(ns)});
    else
      throw cspec::cli::invalid_namespace(ns);
  return captured;
}

void cspec::cli::list(argparse::ArgumentParser args)
{
  const auto format = args.get<cspec::cli::format_t>("format");
  const auto query = args.get<vector<string>>("query");

  if (contains_icase(query, "all.all"))
    stream(format, cspec::cli::queries);
  else if (contains_icase(query, "all"))
    stream(format, cspec::cli::namespaces);
  else
    stream(format, capture(query));
}

/**
 *  @file      get.cpp
 *  @brief     Make queries in all or select namespaces.
 *
 *  @author    Evan Elias Young
 *  @date      2021-11-25
 *  @date      2021-11-25
 *  @copyright Copyright 2021 Evan Elias Young. All rights reserved.
 */

#include "../cspec.h"
#include "ns.h"

std::map<string, vector<string>> capture(const vector<string> &query)
{
  if (contains_icase(query, "all.all") || contains_icase(query, "all"))
    return cspec::cli::queries;

  std::map<string, vector<string>> captured = {};
  for (const auto &coll: query)
  {
    const auto per_idx = coll.find_first_of('.');
    const auto ns = per_idx == string::npos ? coll : coll.substr(0, per_idx);
    const auto key = per_idx == string::npos ? "all" : coll.substr(per_idx + 1);

    if (contains_icase(cspec::cli::namespaces, ns))
      if (icaseis(key, "all") || contains_icase(cspec::cli::queries.at(ns), key))
      {
        if (captured.find(ns) == captured.end())
          captured.insert({ns, {}});
        if (icaseis(key, "all"))
          captured.at(ns) = cspec::cli::queries.at(ns);
        else if (!contains_icase(captured.at(ns), key))
          captured.at(ns).push_back(key);
      }
      else
        throw cspec::cli::invalid_query(ns, key);
    else
      throw cspec::cli::invalid_namespace(ns);
  }
  return captured;
}

void cspec::cli::get(argparse::ArgumentParser args)
{
  const auto format = args.get<cspec::cli::format_t>("format");
  const auto captured = capture(args.get<vector<string>>("query"));
  auto j = cspec::collect(captured);

  if (args.get<bool>("human"))
    cspec::cli::human_friendly_json(j);

  if (format == cspec::cli::format_t::json)
    std::cout << j.dump(2) << '\n';
  else
  {
    auto pre = vector<string>{};
    stream(format, pre, j);
  }
}

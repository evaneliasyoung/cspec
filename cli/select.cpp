/**
 *  @file      select.cpp
 *  @brief     Handles selecting entries from a query.
 *
 *  @author    Evan Elias Young
 *  @date      2021-11-29
 *  @date      2021-11-29
 *  @copyright Copyright 2021 Evan Elias Young. All rights reserved.
 */

#include "ns.h"

std::map<string, vector<string>> cspec::cli::select(const vector<string> &query)
{
  if (cspec::shared::contains_icase(query, "all.all") || cspec::shared::contains_icase(query, "all"))
    return cspec::cli::queries;

  std::map<string, vector<string>> selected = {};
  for (const auto &coll: query)
  {
    const auto per_idx = coll.find_first_of('.');
    const auto ns = per_idx == string::npos ? coll : coll.substr(0, per_idx);
    const auto key = per_idx == string::npos ? "all" : coll.substr(per_idx + 1);

    if (cspec::shared::contains_icase(cspec::cli::namespaces, ns))
      if (cspec::shared::icaseis(key, "all") || cspec::shared::contains_icase(cspec::cli::queries.at(ns), key))
        if (cspec::shared::icaseis(key, "all"))
          selected.insert_or_assign(ns, cspec::cli::queries.at(ns));
        else if (selected.find(ns) == selected.end())
          selected.insert({ns, {}});
        else if (!cspec::shared::contains_icase(selected.at(ns), key))
          selected.at(ns).push_back(key);
        else
          continue;
      else
        throw cspec::cli::invalid_query(ns, key);
    else
      throw cspec::cli::invalid_namespace(ns);
  }
  return selected;
}

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

void stream(const cspec::cli::format_t &format, const std::map<string, std::map<string, string>> &queries)
{
  const auto del = format == cspec::cli::format_t::compact ? '=' : '\n';
  if (format == cspec::cli::format_t::json)
    std::cout << json(queries).dump(2) << '\n';
  else
    for (const auto &[ns, keys]: queries)
      for (const auto &[key, val]: keys)
        std::cout << ns << '.' << key << del << val << '\n';
}

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

void stream_json_sub(const cspec::cli::format_t &format, vector<string> &pre, const json &j)
{
  if (j.is_object())
  {
    for (const auto &[subkey, subval]: j.items())
    {
      pre.push_back(subkey);
      stream_json_sub(format, pre, j[subkey]);
      pre.pop_back();
    }
  }
  else if (j.is_array())
  {
    for (umax i = 0; i < j.size(); ++i)
    {
      pre.push_back("[" + std::to_string(i) + "]");
      stream_json_sub(format, pre, j[i]);
      pre.pop_back();
    }
  }
  else if (format != cspec::cli::format_t::value)
  {
    for (const auto &key: pre)
      std::cout << (key.back() == ']' ? "\b" : "") << key << '.';
    std::cout << '\b' << (format == cspec::cli::format_t::compact ? "=" : " \n") << j << '\n';
  }
  else
    std::cout << j << '\n';
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
    stream_json_sub(format, pre, j);
  }
}

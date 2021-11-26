/**
 *  @file      stream.cpp
 *  @brief     Handles streaming output to cspec cli.
 *
 *  @author    Evan Elias Young
 *  @date      2021-11-25
 *  @date      2021-11-25
 *  @copyright Copyright 2021 Evan Elias Young. All rights reserved.
 */

#include "ns.h"

void cspec::cli::stream(const cspec::cli::format_t &format, vector<string> &pre, const json &j)
{
  if (j.is_primitive())
    if (format != cspec::cli::format_t::value)
    {
      for (const auto &key: pre)
        std::cout << (key.back() == ']' ? "\b" : "") << key << '.';
      std::cout << '\b' << cspec::cli::format_delimiters.at(format) << j << '\n';
    }
    else
      std::cout << j << '\n';
  else if (j.is_object())
    for (const auto &[subkey, subval]: j.items())
    {
      pre.push_back(subkey);
      cspec::cli::stream(format, pre, j[subkey]);
      pre.pop_back();
    }
  else if (j.is_array())
    for (umax i = 0; i < j.size(); ++i)
    {
      pre.push_back("[" + std::to_string(i) + "]");
      cspec::cli::stream(format, pre, j[i]);
      pre.pop_back();
    }
}

void cspec::cli::stream(const cspec::cli::format_t &format, const std::map<string, vector<string>> &queries)
{
  if (format == cspec::cli::format_t::json)
    std::cout << json(queries).dump(2) << '\n';
  else
    for (const auto &[ns, keys]: queries)
      for (const auto &key: keys)
        std::cout << ns << '.' << key << '\n';
}

void cspec::cli::stream(const cspec::cli::format_t &format, const vector<string> &namespaces)
{
  if (format == cspec::cli::format_t::json)
    std::cout << json(namespaces).dump(2) << '\n';
  else
    for (const auto &ns: namespaces)
      std::cout << ns << '\n';
}

void stream(const cspec::cli::format_t &format, const std::map<string, std::map<string, string>> &queries)
{
  if (format == cspec::cli::format_t::json)
    std::cout << json(queries).dump(2) << '\n';
  else
    for (const auto &[ns, keys]: queries)
      for (const auto &[key, val]: keys)
        std::cout << ns << '.' << key << cspec::cli::format_delimiters.at(format) << val << '\n';
}

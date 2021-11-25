/**
 *  @file      stream.cpp
 *  @brief     Handles streaming output.
 *
 *  @author    Evan Elias Young
 *  @date      2021-11-25
 *  @date      2021-11-25
 *  @copyright Copyright 2021 Evan Elias Young. All rights reserved.
 */

#include "ns.h"

void cspec::cli::stream(const json&j)
{
  std::cout << j.dump(2) << '\n';
}

void cspec::cli::stream(const vector<string> &values)
{
  for (const auto &entry: values)
    std::cout << entry << '\n';
}

void cspec::cli::stream(const cspec::cli::format_t &format, const vector<string> &values)
{
  if (format == cspec::cli::format_t::json)
    std::cout << json(values).dump(2) << '\n';
  else
    cspec::cli::stream(values);
}

void cspec::cli::stream(const cspec::cli::format_t &format, const std::map<string, string> &keyval)
{
  std::function<void(const string &key, const string &val)> streamer;
  if (format == cspec::cli::format_t::compact)
    streamer = [](const string &key, const string &val)
    {
      std::cout << key << '=' << val;
    };
  else if (format == cspec::cli::format_t::list)
    streamer = [](const string &key, const string &val)
    {
      std::cout << key << '\n' << val;
    };
  else if (format == cspec::cli::format_t::value)
    streamer = [](const string &key, const string &val)
    {
      std::cout << val;
    };
}

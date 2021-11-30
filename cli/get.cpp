/**
 *  @file      get.cpp
 *  @brief     Make queries in all or select namespaces.
 *
 *  @author    Evan Elias Young
 *  @date      2021-11-25
 *  @date      2021-11-29
 *  @copyright Copyright 2021 Evan Elias Young. All rights reserved.
 */

#include "../cspec.h"
#include "ns.h"

void cspec::cli::get(argparse::ArgumentParser args)
{
  const auto format = args.get<cspec::cli::format_t>("format");
  const auto selected = cspec::cli::select(args.get<vector<string>>("query"));
  auto j = cspec::collect(selected);

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

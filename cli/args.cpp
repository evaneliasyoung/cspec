/**
 *  @file      args.cpp
 *  @brief     Argument parser for cspec cli.
 *
 *  @author    Evan Elias Young
 *  @date      2021-11-25
 *  @date      2021-11-25
 *  @copyright Copyright 2021 Evan Elias Young. All rights reserved.
 */

#include "ns.h"

argparse::ArgumentParser cspec::cli::parse_args(int argc, char const *argv[])
{
  argparse::ArgumentParser args("cspec", "0.1.0");

  args.add_description("retrieve system specifications and hardware information");
  args.add_epilog("thanks for supporting open source software! ~EEY");

  args.add_argument("--format", "-f")
    .default_value(cspec::cli::format_t::list)
    .action(
      [](const string &val) -> cspec::cli::format_t
      {
        if (val == "list")
          return cspec::cli::format_t::list;
        else if (val == "compact")
          return cspec::cli::format_t::compact;
        else if (val == "value")
          return cspec::cli::format_t::value;
        else if (val == "json")
          return cspec::cli::format_t::json;
        else
          throw std::invalid_argument("invalid format; must be one of 'list', 'compact', 'value' (only), 'json'");
      })
    .help("set output format; one of 'list', 'compact', 'value' (only), 'json'")
    .nargs(1);

  args.add_argument("--human", "-H").default_value(false).implicit_value(true).nargs(0).help(
    "displays values in a human-friendly format");

  args.add_argument("action")
    .help("query or list available queries; one of 'get', 'list'")
    .required()
    .action(
      [](const string &val) -> string
      {
        if (!(val == "list" || val == "get"))
          throw std::invalid_argument("invalid action; must be one of 'get', 'list'");
        return val;
      });

  args.add_argument("query").help("specific query or namespace").default_value(vector<string>{"all.all"}).remaining();

  try
  {
    args.parse_args(argc, argv);
  }
  catch (const std::runtime_error &err)
  {
    std::cerr << err.what() << std::endl;
    std::cerr << args;
    std::exit(1);
  }

  return args;
}

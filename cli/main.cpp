/**
 *  @file      main.cpp
 *  @brief     Main point of entry for the cspec cli.
 *
 *  @author    Evan Elias Young
 *  @date      2021-11-25
 *  @date      2021-11-25
 *  @copyright Copyright 2021 Evan Elias Young. All rights reserved.
 */

#include "ns.h"

void cspec::cli::run(argparse::ArgumentParser args)
{
  if (args.get("action") == "list")
    cspec::cli::list(args);
  else
    cspec::cli::get(args);
}

void cspec::cli::main(int argc, char const *argv[])
{
  cspec::cli::run(cspec::cli::parse_args(argc, argv));
}

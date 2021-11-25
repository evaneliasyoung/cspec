/**
 *  @file      get.cpp
 *  @brief     Make queries in all or select namespaces.
 *
 *  @author    Evan Elias Young
 *  @date      2021-11-25
 *  @date      2021-11-25
 *  @copyright Copyright 2021 Evan Elias Young. All rights reserved.
 */

#include "ns.h"

void cspec::cli::get(argparse::ArgumentParser args)
{
  for (const auto &key: args.get("query"))
    std::cout << key << '\n';
}

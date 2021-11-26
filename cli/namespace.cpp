/**
 *  @file      namespace.cpp
 *  @brief     Handles namespace conversion and iteration.
 *
 *  @author    Evan Elias Young
 *  @date      2021-11-25
 *  @date      2021-11-25
 *  @copyright Copyright 2021 Evan Elias Young. All rights reserved.
 */

#include "ns.h"

cspec::cli::namespace_t cspec::cli::stons(const string &ns)
{
  if (icaseis(ns, "cpu"))
    return cspec::cli::namespace_t::cpu;
  if (icaseis(ns, "filesystem"))
    return cspec::cli::namespace_t::filesystem;
  if (icaseis(ns, "gpu"))
    return cspec::cli::namespace_t::gpu;
  if (icaseis(ns, "memory"))
    return cspec::cli::namespace_t::memory;
  if (icaseis(ns, "system"))
    return cspec::cli::namespace_t::system;
  throw cspec::cli::invalid_namespace(ns);
}

string cspec::cli::nstos(const cspec::cli::namespace_t &ns)
{
  switch (ns)
  {
    case cspec::cli::namespace_t::cpu:
      return "cpu";
    case cspec::cli::namespace_t::filesystem:
      return "filesystem";
    case cspec::cli::namespace_t::gpu:
      return "gpu";
    case cspec::cli::namespace_t::memory:
      return "memory";
    case cspec::cli::namespace_t::system:
      return "system";
    default:
      return "unknown";
  }
}

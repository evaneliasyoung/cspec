/**
 *  @file      strconv.cpp
 *  @brief     Handles system related string conversions.
 *
 *  @author    Evan Elias Young
 *  @date      2021-11-29
 *  @date      2021-11-29
 *  @copyright Copyright 2021 Evan Elias Young. All rights reserved.
 */

#include "../shared/ns.h"
#include "ns.h"

cspec::system::kernel_t cspec::system::stokrn(const string &kernel)
{
  if (cspec::shared::icaseis(kernel, "Windows NT"))
    return cspec::system::kernel_t::nt;
  else if (cspec::shared::icaseis(kernel, "Linux"))
    return cspec::system::kernel_t::linux;
  else if (cspec::shared::icaseis(kernel, "Darwin"))
    return cspec::system::kernel_t::darwin;
  else
    return cspec::system::kernel_t::unknown;
}

string cspec::system::krntos(const cspec::system::kernel_t &kernel)
{
  switch (kernel)
  {
    case cspec::system::kernel_t::nt:
      return "Windows NT";
    case cspec::system::kernel_t::linux:
      return "Linux";
    case cspec::system::kernel_t::darwin:
      return "Darwin";
    default:
      return "Unknown";
  }
}

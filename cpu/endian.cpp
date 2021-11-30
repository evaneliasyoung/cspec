/**
 *  @file      endian.cpp
 *  @brief     Determines cpu litte/big endian.
 *
 *  @author    Evan Elias Young
 *  @date      2021-11-11
 *  @date      2021-11-29
 *  @copyright Copyright 2021 Evan Elias Young. All rights reserved.
 */

#include "../shared/ns.h"
#include "ns.h"

cspec::cpu::endian_t cspec::cpu::stoend(const string &endian_type)
{
  if (cspec::shared::icaseis(endian_type, "Big Endian"))
    return cspec::cpu::endian_t::big;
  return cspec::cpu::endian_t::little;
}

string cspec::cpu::endtos(const cspec::cpu::endian_t &endian_type)
{
  switch (endian_type)
  {
    case cspec::cpu::endian_t::big:
      return "big";
    case cspec::cpu::endian_t::little:
      return "little";
    default:
      return "unknown";
  }
}

cspec::cpu::endian_t cspec::cpu::endian() noexcept
{
  const u16 test = 0xFF00;
  const auto result = *static_cast<const u8 *>(static_cast<const void *>(&test));

  if (result == 0xFF)
    return cspec::cpu::endian_t::big;
  return cspec::cpu::endian_t::little;
}

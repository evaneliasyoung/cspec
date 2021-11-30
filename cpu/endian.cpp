/**
 *  @file      endian.cpp
 *  @brief     Determines cpu litte/big endian.
 *
 *  @author    Evan Elias Young
 *  @date      2021-11-11
 *  @date      2021-11-29
 *  @copyright Copyright 2021 Evan Elias Young. All rights reserved.
 */

#include "ns.h"

cspec::cpu::endian_t cspec::cpu::endian() noexcept
{
  const u16 test = 0xFF00;
  const auto result = *static_cast<const u8 *>(static_cast<const void *>(&test));

  if (result == 0xFF)
    return cspec::cpu::endian_t::big;
  return cspec::cpu::endian_t::little;
}

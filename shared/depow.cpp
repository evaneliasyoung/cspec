/**
 *  @file      depow.cpp
 *  @brief     Swaps *bibits, *binibbles and *bibytes.
 *
 *  @author    Evan Elias Young
 *  @date      2021-11-29
 *  @date      2021-11-29
 *  @copyright Copyright 2021 Evan Elias Young. All rights reserved.
 */

#include "ns.h"

umax cspec::shared::depow2(const umax &bi)
{
  double mod = 1;
  for (umax i = 0; i < (umax)std::floor(std::log(bi) / std::log(1024)); ++i)
    mod *= 1000.00 / 1024.00;
  return (umax)std::round(mod * bi);
}

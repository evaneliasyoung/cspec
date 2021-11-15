/**
 *  @file      ns.h
 *  @brief     Namspace extensions for cspec CPU.
 *
 *  @author    Evan Elias Young
 *  @date      2021-11-15
 *  @date      2021-11-15
 *  @copyright Copyright 2021 Evan Elias Young. All rights reserved.
 */

#pragma once

#include "../pch.h"

namespace cspec
{
  namespace cpu
  {
    enum class architecture_t
    {
      x64,
      arm,
      itanium,
      x86,
      unknown
    };

    enum class endian_t
    {
      little,
      big
    };

    enum class cache_type_t
    {
      unified,
      instruction,
      data,
      trace
    };

    struct amounts_t
    {
      // Logical cores.
      u32 threads;
      // Physical cores.
      u32 cores;
      // Physical CPU sockets.
      u32 packages;
    };

    struct cache_t
    {
      umax size;
      umax line_size;
      u8 association;
      cache_type_t type;
    };

    amounts_t amounts();
    cache_t cache(u8 level);
    architecture_t architecture();
    u64 clock() noexcept;
    endian_t endian() noexcept;
    std::string vendor();
    std::string vendor_id();
    std::string name();
  } // namespace cpu
} // namespace cspec

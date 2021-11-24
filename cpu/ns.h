/**
 *  @file      ns.h
 *  @brief     Namspace extensions for cspec CPU.
 *
 *  @author    Evan Elias Young
 *  @date      2021-11-15
 *  @date      2021-11-20
 *  @copyright Copyright 2021 Evan Elias Young. All rights reserved.
 */

#pragma once

#include "../core.h"

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

    struct group_t
    {
      umax family;
      umax model;
      umax stepping;
    };

    amounts_t amounts();
    cache_t cache(u8 level);
    architecture_t architecture();
    u64 clock() noexcept;
    endian_t endian() noexcept;
    string vendor();
    string name();
    group_t group();
    architecture_t string_to_architecture(const string &architecture);
    string architecture_to_string(const architecture_t &arch);
  } // namespace cpu
} // namespace cspec

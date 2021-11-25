/**
 *  @file      ns.h
 *  @brief     Namspace extensions for cspec CPU.
 *
 *  @author    Evan Elias Young
 *  @date      2021-11-15
 *  @date      2021-11-24
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
    architecture_t stoarch(const string &architecture);
    string archtos(const architecture_t &architecture);

    enum class endian_t
    {
      little,
      big
    };
    endian_t stoend(const string &endian_type);
    string endtos(const endian_t &endian_type);

    enum class cache_type_t
    {
      unified,
      instruction,
      data,
      trace,
      unknown
    };
    cache_type_t stocch(const string &cache_type);
    string cchtos(const cache_type_t &cache_type);

    struct amounts_t
    {
      // Logical cores.
      u32 threads;
      // Physical cores.
      u32 cores;
      // Physical CPU sockets.
      u32 packages;
    };
    void to_json(json &j, const amounts_t &amt);
    void from_json(const json &j, amounts_t &amt);

    struct cache_t
    {
      umax size;
      umax line_size;
      u8 association;
      cache_type_t type;
    };
    void to_json(json &j, const cache_t &cch);
    void from_json(const json &j, cache_t &cch);

    struct group_t
    {
      umax family;
      umax model;
      umax stepping;
    };
    void to_json(json &j, const group_t &grp);
    void from_json(const json &j, group_t &grp);

    amounts_t amounts();
    cache_t cache(u8 level);
    architecture_t architecture();
    u64 clock() noexcept;
    endian_t endian() noexcept;
    string vendor();
    string name();
    group_t group();
    json collect();
    json collect(const vector<string> &keys);
  } // namespace cpu
} // namespace cspec

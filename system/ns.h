/**
 *  @file      ns.h
 *  @brief     Namspace extensions for cspec System.
 *
 *  @author    Evan Elias Young
 *  @date      2021-11-17
 *  @date      2021-11-24
 *  @copyright Copyright 2021 Evan Elias Young. All rights reserved.
 */

#pragma once

#include "../core.h"
#include "../shared/ns.h"

#include <chrono>

#ifdef linux
#undef linux
#endif

namespace cspec
{
  namespace system
  {
    struct os_info_t
    {
      string name;
      string full_name;
      cspec::shared::version_t version;
    };
    void to_json(json &j, const os_info_t &os);
    void from_json(const json &j, os_info_t &os);

    enum class kernel_t
    {
      nt,
      linux,
      darwin,
      unknown
    };
    kernel_t stokrn(const string &kernel);
    string krntos(const kernel_t &kernel);

    struct kernel_info_t
    {
      kernel_t type;
      cspec::shared::version_t version;
    };
    void to_json(json &j, const kernel_info_t &krn);
    void from_json(const json &j, kernel_info_t &krn);

    struct times_t
    {
      std::chrono::system_clock::time_point current;
      std::chrono::system_clock::time_point install;
      std::chrono::system_clock::time_point boot;
    };
    void to_json(json &j, const times_t &tms);
    void from_json(const json &j, times_t &tms);

    os_info_t os();
    kernel_info_t kernel();
    times_t times();

  } // namespace system
} // namespace cspec

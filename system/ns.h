/**
 *  @file      ns.h
 *  @brief     Namspace extensions for cspec System.
 *
 *  @author    Evan Elias Young
 *  @date      2021-11-17
 *  @date      2021-11-19
 *  @copyright Copyright 2021 Evan Elias Young. All rights reserved.
 */

#pragma once

#include "../pch.h"

#ifdef linux
#undef linux
#endif

namespace cspec
{
  namespace system
  {
    struct os_info_t
    {
      std::string name;
      std::string full_name;
      u32 major;
      u32 minor;
      u32 patch;
      u32 build;
    };

    enum class kernel_t
    {
      nt,
      linux,
      darwin,
      unknown
    };

    struct kernel_info_t
    {
      kernel_t type;
      u32 major;
      u32 minor;
      u32 patch;
      u32 build;
    };

    struct times_t
    {
      std::chrono::system_clock::time_point current;
      std::chrono::system_clock::time_point boot;
    };

    os_info_t os();
    kernel_info_t kernel();
    times_t times();
  } // namespace system
} // namespace cspec

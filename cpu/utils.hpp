/**
 *  @file      utils.hpp
 *  @brief     Utilities for cpu info.
 *
 *  @author    Evan Elias Young
 *  @date      2021-11-16
 *  @date      2021-11-19
 *  @copyright Copyright 2021 Evan Elias Young. All rights reserved.
 */

#pragma once

#include "../pch.h"

#if defined(WIN)
static std::vector<SYSTEM_LOGICAL_PROCESSOR_INFORMATION> cpuinfo_buffer()
{
  std::vector<SYSTEM_LOGICAL_PROCESSOR_INFORMATION> ret;

  DWORD bytes = 0;
  GetLogicalProcessorInformation(nullptr, &bytes);
  ret.resize(bytes / sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION));
  GetLogicalProcessorInformation(ret.data(), &bytes);

  return ret;
}
#elif defined(MAC)
#else
static std::string read_cpuinfo(const char *key)
{
  std::ifstream cpuinfo("/proc/cpuinfo");

  if (!cpuinfo.is_open() || !cpuinfo)
    return {};

  for (std::string line; std::getline(cpuinfo, line);)
  {
    if (line.find(key) == 0)
    {
      const umax col_idx = line.find_first_of(':');
      const umax nws_idx = line.find_first_not_of(" \t", col_idx + 1);
      return line.c_str() + nws_idx;
    }
  }

  return {};
}
#endif

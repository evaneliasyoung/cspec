/**
 *  @file      utils.hpp
 *  @brief     Utilities for cpu info.
 *
 *  @author    Evan Elias Young
 *  @date      2021-11-16
 *  @date      2021-11-16
 *  @copyright Copyright 2021 Evan Elias Young. All rights reserved.
 */

#include "../pch.h"

#if defined(WINDOWS)
static std::vector<SYSTEM_LOGICAL_PROCESSOR_INFORMATION> cpuinfo_buffer()
{
  std::vector<SYSTEM_LOGICAL_PROCESSOR_INFORMATION> ret;

  DWORD bytes = 0;
  GetLogicalProcessorInformation(nullptr, &bytes);
  ret.resize(bytes / sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION));
  GetLogicalProcessorInformation(ret.data(), &bytes);

  return ret;
}

template<unsigned int BufferSize> static std::string read_cpu_registry(const char *key)
{
  HKEY hkey;
  if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, R"(HARDWARE\DESCRIPTION\System\CentralProcessor\0)", 0, KEY_READ, &hkey))
    return {};

  char identifier[BufferSize];
  DWORD identifier_len = sizeof(identifier);
  LPBYTE lpdata = static_cast<LPBYTE>(static_cast<void *>(&identifier[0]));
  if (RegQueryValueExA(hkey, key, nullptr, nullptr, lpdata, &identifier_len))
    return {};

  return identifier;
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
      const auto col_idx = line.find_first_of(':');
      const auto nws_idx = line.find_first_not_of(" \t", col_idx + 1);
      return line.c_str() + nws_idx;
    }
  }

  return {};
}
#endif

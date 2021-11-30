/**
 *  @file      clock.cpp
 *  @brief     Determines cpu clock speed.
 *
 *  @author    Evan Elias Young
 *  @date      2021-11-15
 *  @date      2021-11-29
 *  @copyright Copyright 2021 Evan Elias Young. All rights reserved.
 */

#include "ns.h"

#if defined(WIN)
#include <windows.h>

u64 cspec::cpu::clock() noexcept
{
  HKEY hkey;
  if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, R"(HARDWARE\DESCRIPTION\System\CentralProcessor\0)", 0, KEY_READ, &hkey))
  {
    LARGE_INTEGER clock;
    QueryPerformanceFrequency(&clock);
    return clock.QuadPart * 1000;
  }

  DWORD clock_mhz;
  DWORD clock_mhz_len = sizeof(clock_mhz);
  if (RegQueryValueExA(hkey, "~MHz", nullptr, nullptr, static_cast<LPBYTE>(static_cast<void *>(&clock_mhz)),
                       &clock_mhz_len))
    return {};

  return clock_mhz * 1000000;
}
#elif defined(MAC)
#include "../shared/ns.h"

u64 cspec::cpu::clock() noexcept
{
  u64 buf;
  cspec::shared::sysctlintegral("hw.cpufrequency", buf);
  return buf;
}
#else
#include <fstream>

u64 cspec::cpu::clock() noexcept
{
  std::ifstream cpuinfo("/proc/cpuinfo");

  if (!cpuinfo.is_open() || !cpuinfo)
    return 0;

  for (string line; std::getline(cpuinfo, line);)
  {
    if (line.find("cpu MHz") == 0)
    {
      const auto col_idx = line.find_first_of(':');
      return static_cast<u64>(std::strtod(line.c_str() + col_idx + 1, nullptr) * 1000000);
    }
  }

  return 0;
}
#endif

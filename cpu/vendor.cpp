/**
 *  @file      vendor.cpp
 *  @brief     Determines cpu vendor and name.
 *
 *  @author    Evan Elias Young
 *  @date      2021-11-15
 *  @date      2021-11-29
 *  @copyright Copyright 2021 Evan Elias Young. All rights reserved.
 */

#include "ns.h"

#if defined(WIN)
#include "../utils/trim.hpp"
#include "../utils/win/registry.hpp"

string cspec::cpu::vendor()
{
  return rtrim(
    read_registry_sz<13>(HKEY_LOCAL_MACHINE, R"(HARDWARE\DESCRIPTION\System\CentralProcessor\0)", "VendorIdentifier"));
}

string cspec::cpu::name()
{
  return rtrim(read_registry_sz<65>(HKEY_LOCAL_MACHINE, R"(HARDWARE\DESCRIPTION\System\CentralProcessor\0)",
                                    "ProcessorNameString"));
}
#elif defined(MAC)
#include "../utils/mac/sysctl.hpp"

string cspec::cpu::vendor()
{
  string vendor;
  if (!sysctlstring("machdep.cpu.vendor", vendor))
    return "Unknown";
  return vendor;
}

string cspec::cpu::name()
{
  string name;

  if (!sysctlstring("machdep.cpu.brand_string", name))
    return "Unknown";
  if (name.find('@') != string::npos)
    name.erase(name.find(" @"));

  return name;
}
#else
#include <fstream>

static string read_cpuinfo(const char *key)
{
  std::ifstream cpuinfo("/proc/cpuinfo");

  if (!cpuinfo.is_open() || !cpuinfo)
    return {};

  for (string line; std::getline(cpuinfo, line);)
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

string cspec::cpu::vendor()
{
  return read_cpuinfo("vendor");
}

string cspec::cpu::name()
{
  return read_cpuinfo("model name");
}
#endif

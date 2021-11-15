/**
 *  @file      vendor.cpp
 *  @brief     Determines cpu vendor and name.
 *
 *  @author    Evan Elias Young
 *  @date      2021-11-15
 *  @date      2021-11-15
 *  @copyright Copyright 2021 Evan Elias Young. All rights reserved.
 */

#include "ns.h"

#if defined(WINDOWS)
template<unsigned int BufferSize> static std::string read_registry(const char *key)
{
  HKEY hkey;
  if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, R"(HARDWARE\DESCRIPTION\System\CentralProcessor\0)", 0, KEY_READ, &hkey))
    return {};

  char identifier[BufferSize];
  DWORD identifier_len = sizeof(identifier);
  LPBYTE lpdata = static_cast<LPBYTE>(STATIC_CAST<void*>(&identifier[0])));
  if (RegQueryValueExA(hkey, key, nullptr, nullptr, lpdata, &identifier_len))
    return {};

  return identifier;
}

std::string cspec::cpu::vendor()
{
  return read_registry<13>("VendorIdentifier");
}

std::string cspec::cpu::name()
{
  return read_registry<65>("ProcessorNameString");
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

std::string cspec::cpu::vendor()
{
  return read_cpuinfo("vendor");
}

std::string cspec::cpu::name()
{
  return read_cpuinfo("model name");
}
#endif

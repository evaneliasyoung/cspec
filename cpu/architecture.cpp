/**
 *  @file      architecture.cpp
 *  @brief     Determines cpu architecture.
 *
 *  @author    Evan Elias Young
 *  @date      2021-11-11
 *  @date      2021-11-20
 *  @copyright Copyright 2021 Evan Elias Young. All rights reserved.
 */

#include "ns.h"

cspec::cpu::architecture_t cspec::cpu::string_to_architecture(const string &architecture)
{
  if (strcasecmp(architecture.c_str(), "x86_64") == 0)
    return cspec::cpu::architecture_t::x64;
  else if (strcasecmp(architecture.c_str(), "i686") == 0)
    return cspec::cpu::architecture_t::x86;
  else if (strstr(architecture.c_str(), "arm") == architecture.c_str())
    return cspec::cpu::architecture_t::arm;
  else if (strcasecmp(architecture.c_str(), "ia64") == 0)
    return cspec::cpu::architecture_t::itanium;
  else
    return cspec::cpu::architecture_t::unknown;
}

string cspec::cpu::architecture_to_string(const cspec::cpu::architecture_t &arch)
{
  switch (arch)
  {
    case cspec::cpu::architecture_t::x64:
      return "x64";
    case cspec::cpu::architecture_t::x86:
      return "x86";
    case cspec::cpu::architecture_t::arm:
      return "ARM";
    case cspec::cpu::architecture_t::itanium:
      return "Itanium";
    default:
      return "Unknown";
  }
}

#ifdef WIN
#include <windows.h>

cspec::cpu::architecture_t cspec::cpu::architecture()
{
  SYSTEM_INFO sysinfo;
  GetNativeSystemInfo(&sysinfo);

  switch (sysinfo.wProcessorArchitecture)
  {
    case PROCESSOR_ARCHITECTURE_AMD64:
      return cspec::cpu::architecture_t::x64;
    case PROCESSOR_ARCHITECTURE_ARM:
      return cspec::cpu::architecture_t::arm;
    case PROCESSOR_ARCHITECTURE_IA64:
      return cspec::cpu::architecture_t::itanium;
    case PROCESSOR_ARCHITECTURE_INTEL:
      return cspec::cpu::architecture_t::x86;
    default:
      return cspec::cpu::architecture_t::unknown;
  }
}
#else
#include <sys/utsname.h>

cspec::cpu::architecture_t cspec::cpu::architecture()
{
  utsname buf;
  if (uname(&buf) == -1)
    return cspec::cpu::architecture_t::unknown;

  return string_to_architecture(buf.machine);
}
#endif

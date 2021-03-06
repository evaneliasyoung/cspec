/**
 *  @file      architecture.cpp
 *  @brief     Determines cpu architecture.
 *
 *  @author    Evan Elias Young
 *  @date      2021-11-11
 *  @date      2021-11-29
 *  @copyright Copyright 2021 Evan Elias Young. All rights reserved.
 */

#include "ns.h"

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

  return stoarch(buf.machine);
}
#endif

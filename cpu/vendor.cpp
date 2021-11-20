/**
 *  @file      vendor.cpp
 *  @brief     Determines cpu vendor and name.
 *
 *  @author    Evan Elias Young
 *  @date      2021-11-15
 *  @date      2021-11-19
 *  @copyright Copyright 2021 Evan Elias Young. All rights reserved.
 */

#include "../utils.h"
#include "ns.h"
#include "utils.hpp"

#if defined(WIN)
std::string cspec::cpu::vendor()
{
  return read_registry_sz<13>(HKEY_LOCAL_MACHINE, R"(HARDWARE\DESCRIPTION\System\CentralProcessor\0)",
                              "VendorIdentifier");
}

std::string cspec::cpu::name()
{
  return read_registry_sz<65>(HKEY_LOCAL_MACHINE, R"(HARDWARE\DESCRIPTION\System\CentralProcessor\0)",
                              "ProcessorNameString");
}
#elif defined(MAC)
#else
std::string cspec::cpu::vendor()
{
  return read_cpuinfo("vendor");
}

std::string cspec::cpu::name()
{
  return read_cpuinfo("model name");
}
#endif

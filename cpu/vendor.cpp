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
#include "utils.hpp"

#if defined(WINDOWS)
std::string cspec::cpu::vendor()
{
  return read_cpu_registry<13>("VendorIdentifier");
}

std::string cspec::cpu::name()
{
  return read_cpu_registry<65>("ProcessorNameString");
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

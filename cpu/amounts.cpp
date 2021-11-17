/**
 *  @file      amounts.cpp
 *  @brief     Determines cpu core amounts.
 *
 *  @author    Evan Elias Young
 *  @date      2021-11-16
 *  @date      2021-11-16
 *  @copyright Copyright 2021 Evan Elias Young. All rights reserved.
 */

#include "ns.h"

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

cspec::cpu::amounts_t cspec::cpu::amounts()
{
  cspec::cpu::amounts_t ret{};
  for (auto &&info: cpuinfo_buffer())
  {
    switch (info.Relationship)
    {
      case RelationProcessorCore:
        ++ret.packages;
        ret.threads += static_cast<std::uint32_t>(
          std::bitset<sizeof(ULONG_PTR) * 8>(static_cast<std::uintptr_t>(info.ProcessorMask)).count());
        break;
      case RelationProcessorPackage:
        ++ret.packages;
        break;
      default:
        break;
    }
  }

  return ret;
}
#elif defined(MAC)
#else
cspec::cpu::amounts_t cspec::cpu::amounts()
{
  cspec::cpu::amounts_t ret{};
  ret.threads = sysconf(_SC_NPROCESSORS_ONLN);

  std::ifstream cpuinfo("/proc/cpuinfo");

  if (!cpuinfo.is_open() || !cpuinfo)
    return ret;

  std::vector<unsigned int> package_ids;
  for (std::string line; std::getline(cpuinfo, line);)
  {
    if (line.find("physical id") == 0)
    {
      const auto physical_id = std::strtoul(line.c_str() + line.find_first_of("1234567890"), nullptr, 10);
      if (std::find(package_ids.begin(), package_ids.end(), physical_id) == package_ids.end())
        package_ids.emplace_back(physical_id);
    }
  }

  ret.packages = package_ids.size();
  ret.cores = ret.threads / ret.packages;

  return ret;
}
#endif

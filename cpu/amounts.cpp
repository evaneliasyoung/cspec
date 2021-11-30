/**
 *  @file      amounts.cpp
 *  @brief     Determines cpu core amounts.
 *
 *  @author    Evan Elias Young
 *  @date      2021-11-16
 *  @date      2021-11-29
 *  @copyright Copyright 2021 Evan Elias Young. All rights reserved.
 */

#include "ns.h"

void cspec::cpu::to_json(json &j, const cspec::cpu::amounts_t &amt)
{
  j = json{{"threads", amt.threads}, {"cores", amt.cores}, {"packages", amt.packages}};
}

void cspec::cpu::from_json(const json &j, cspec::cpu::amounts_t &amt)
{
  j.at("threads").get_to(amt.threads);
  j.at("cores").get_to(amt.cores);
  j.at("packages").get_to(amt.packages);
}

#if defined(WIN)
#include "../shared/ns.h"

#include <bitset>

cspec::cpu::amounts_t cspec::cpu::amounts()
{
  cspec::cpu::amounts_t ret{};
  for (auto &&info: cspec::shared::cpuinfo_buffer())
  {
    switch (info.Relationship)
    {
      case RelationProcessorCore:
        ++ret.cores;
        ret.threads +=
          static_cast<u32>(std::bitset<sizeof(ULONG_PTR) * 8>(static_cast<uptr>(info.ProcessorMask)).count());
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
#include "../shared/ns.h"

cspec::cpu::amounts_t cspec::cpu::amounts()
{
  cspec::cpu::amounts_t ret{};

  cspec::shared::sysctlintegral("hw.physicalcpu", ret.cores);
  cspec::shared::sysctlintegral("hw.logicalcpu", ret.threads);
  cspec::shared::sysctlintegral("hw.packages", ret.packages);

  return ret;
}
#else
#include <algorithm>
#include <fstream>
#include <unistd.h>
#include <vector>

cspec::cpu::amounts_t cspec::cpu::amounts()
{
  cspec::cpu::amounts_t ret{};
  ret.threads = sysconf(_SC_NPROCESSORS_ONLN);

  std::ifstream cpuinfo("/proc/cpuinfo");

  if (!cpuinfo.is_open() || !cpuinfo)
    return ret;

  vector<u32> package_ids;
  for (string line; std::getline(cpuinfo, line);)
  {
    if (line.find("physical id") == 0)
    {
      const u64 physical_id = std::strtoul(line.c_str() + line.find_first_of("1234567890"), nullptr, 10);
      if (std::find(package_ids.begin(), package_ids.end(), physical_id) == package_ids.end())
        package_ids.emplace_back(physical_id);
    }
  }

  ret.packages = package_ids.size();
  ret.cores = ret.threads / ret.packages;

  return ret;
}
#endif

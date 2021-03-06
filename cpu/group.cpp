/**
 *  @file      group.cpp
 *  @brief     Determines group info for cpu.
 *
 *  @author    Evan Elias Young
 *  @date      2021-11-23
 *  @date      2021-11-29
 *  @copyright Copyright 2021 Evan Elias Young. All rights reserved.
 */

#include "ns.h"

#if defined(WIN)
#include "../shared/ns.h"

cspec::cpu::group_t cspec::cpu::group()
{
  cspec::shared::WMI wmi;
  if (!wmi.prepare())
    return {};

  const auto group_string = wmi.query_and_retrieve<string>("Win32_Processor", {"Caption"}).at("Caption");
  std::smatch match;

  if (!std::regex_search(group_string, match, R"(Family (\d+) Model (\d+) Stepping (\d+))"_regex))
    return {};

  umax iter = 0;
  const auto family = std::stoul(match[++iter]);
  const auto model = std::stoul(match[++iter]);
  const auto stepping = std::stoul(match[++iter]);
  return {family, model, stepping};
}
#elif defined(MAC)
#include "../shared/ns.h"

cspec::cpu::group_t cspec::cpu::group()
{
  cspec::cpu::group_t ret{};

  cspec::shared::sysctlintegral("machdep.cpu.family", ret.family);
  cspec::shared::sysctlintegral("machdep.cpu.model", ret.model);
  cspec::shared::sysctlintegral("machdep.cpu.stepping", ret.stepping);

  return ret;
}
#else
#include <fstream>

cspec::cpu::group_t cspec::cpu::group()
{
  cspec::cpu::group_t ret{};
  std::ifstream cpuinfo("/proc/cpuinfo");

  if (!cpuinfo.is_open() || !cpuinfo)
    return ret;

  const auto line_to_ul = [](const string line) -> umax
  {
    return std::strtoul(line.c_str() + line.find_first_of("1234567890"), nullptr, 10);
  };
  for (string line; std::getline(cpuinfo, line);)
  {
    if (line.find("cpu family") == 0)
      ret.family = line_to_ul(line);
    if (line.find("stepping") == 0)
      ret.stepping = line_to_ul(line);
    if (line.find("model") == 0)
      ret.model = line_to_ul(line);
  }

  return ret;
}
#endif

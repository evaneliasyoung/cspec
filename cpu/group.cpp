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

void cspec::cpu::to_json(json &j, const cspec::cpu::group_t &grp)
{
  j = json{{"family", grp.family}, {"model", grp.model}, {"stepping", grp.stepping}};
}

void cspec::cpu::from_json(const json &j, cspec::cpu::group_t &grp)
{
  j.at("family").get_to(grp.family);
  j.at("model").get_to(grp.model);
  j.at("stepping").get_to(grp.stepping);
}

#if defined(WIN)
#include "../utils/win/wmi.hpp"

cspec::cpu::group_t cspec::cpu::group()
{
  WMI wmi;
  if (!wmi.prepare())
    return {};

  const auto group_string = wmi.query_and_retrieve<string>("Win32_Processor", {"Caption"}).at("Caption");
  std::smatch match;

  if (!std::regex_search(group_string, match, R"(Family (\d+) Model (\d+) Stepping (\d+))"_regex))
    return {};

  auto iter = 0;
  const auto family = std::stoul(match[++iter]);
  const auto model = std::stoul(match[++iter]);
  const auto stepping = std::stoul(match[++iter]);
  return {family, model, stepping};
}
#elif defined(MAC)
cspec::cpu::group_t cspec::cpu::group()
{
  cspec::cpu::group_t ret{};

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

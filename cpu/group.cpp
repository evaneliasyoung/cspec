/**
 *  @file      group.cpp
 *  @brief     Determines group info for cpu.
 *
 *  @author    Evan Elias Young
 *  @date      2021-11-23
 *  @date      2021-11-23
 *  @copyright Copyright 2021 Evan Elias Young. All rights reserved.
 */

#include "ns.h"

#if defined(WIN)
#include "../utils/win/wmi.hpp"

#include <regex>

cspec::cpu::group_t cspec::cpu::group()
{
  WMI wmi;
  if (!wmi.prepare())
    return {};

  const auto group_string = wmi.query_and_retrieve<string>("Win32_Processor", "Caption");
  std::smatch match;

  if (!std::regex_search(group_string, match, std::regex(R"(Family (\d+) Model (\d+) Stepping (\d+))")))
    return {};

  auto iter = 0;
  const auto family = std::stoul(match[++iter]);
  const auto model = std::stoul(match[++iter]);
  const auto stepping = std::stoul(match[++iter]);
  return {family, model, stepping};
}
#elif defined(MAC)
#else
#endif

/**
 *  @file      stringtime.cpp
 *  @brief     Handles string-time and time-string casting.
 *
 *  @author    Evan Elias Young
 *  @date      2021-11-29
 *  @date      2021-11-29
 *  @copyright Copyright 2021 Evan Elias Young. All rights reserved.
 */

#include "ns.h"

string cspec::shared::tmtos(const std::chrono::system_clock::time_point &time, const char *fmt)
{
  auto tt = std::chrono::system_clock::to_time_t(time);
  auto tm = *std::gmtime(&tt);
  std::stringstream ss;
  ss << std::put_time(&tm, fmt);
  return ss.str();
}

std::chrono::system_clock::time_point cspec::shared::stotm(const string &time, const char *fmt)
{
  auto tm = *std::gmtime(nullptr);
  std::stringstream ss(time);
  ss >> std::get_time(&tm, fmt);
  return std::chrono::system_clock::from_time_t(std::mktime(&tm));
}

/**
 *  @file      times.cpp
 *  @brief     Determines system time and uptime.
 *
 *  @author    Evan Elias Young
 *  @date      2021-11-19
 *  @date      2021-11-20
 *  @copyright Copyright 2021 Evan Elias Young. All rights reserved.
 */

#include "ns.h"

string tmtos(const std::chrono::system_clock::time_point &time)
{
  auto tt = std::chrono::system_clock::to_time_t(time);
  auto tm = *std::gmtime(&tt);
  std::stringstream ss;
  ss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
  return ss.str();
}

std::chrono::system_clock::time_point stotm(const string &time)
{
  auto tm = *std::gmtime(nullptr);
  std::stringstream ss(time);
  ss >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S");
  return std::chrono::system_clock::from_time_t(std::mktime(&tm));
}

void cspec::system::to_json(json &j, const cspec::system::times_t &tms)
{
  j = json{{"current", tmtos(tms.current)},
           {"install", tmtos(tms.install)},
           {"boot", tmtos(tms.boot)}};
}

void cspec::system::from_json(const json &j, cspec::system::times_t &tms)
{
  tms.current = stotm(j.at("current"));
  tms.install = stotm(j.at("install"));
  tms.boot = stotm(j.at("boot"));
}

#if defined(WIN)
#include "../utils/win/registry.hpp"

#include <windows.h>

cspec::system::times_t cspec::system::times()
{
  const auto cur_time = std::chrono::system_clock::now();
  const auto uptime = std::chrono::milliseconds(GetTickCount64());
  const auto boot_time = cur_time - uptime;

  const auto since_install =
    read_registry_dw(HKEY_LOCAL_MACHINE, R"(SOFTWARE\Microsoft\Windows NT\CurrentVersion)", "InstallDate");
  const auto install_time = std::chrono::system_clock::from_time_t(since_install);

  return {cur_time, install_time, boot_time};
}
#elif defined(MAC)
cspec::system::times_t cspec::system::times()
{
  return {};
}
#else
#include <sys/stat.h>
#include <sys/sysinfo.h>

cspec::system::times_t cspec::system::times()
{
  struct sysinfo info;
  const auto cur_time = std::chrono::system_clock::now();
  const auto uptime = std::chrono::seconds(sysinfo(&info) == 0 ? info.uptime : 0);
  const auto boot_time = cur_time - uptime;

  struct stat attr;
  const auto since_install = stat("/boot/grub", &attr) == 0 ? attr.st_mtim.tv_sec : 0;
  const auto install_time = std::chrono::system_clock::from_time_t(since_install);

  return {cur_time, install_time, boot_time};
}
#endif

/**
 *  @file      times.cpp
 *  @brief     Determines system time and uptime.
 *
 *  @author    Evan Elias Young
 *  @date      2021-11-19
 *  @date      2021-11-29
 *  @copyright Copyright 2021 Evan Elias Young. All rights reserved.
 */

#include "ns.h"

#if defined(WIN)
#include "../shared/ns.h"

#include <windows.h>

cspec::system::times_t cspec::system::times()
{
  const auto cur_time = std::chrono::system_clock::now();
  const auto uptime = std::chrono::milliseconds(GetTickCount64());
  const auto boot_time = cur_time - uptime;

  const auto since_install = cspec::shared::read_registry_dw(
    HKEY_LOCAL_MACHINE, R"(SOFTWARE\Microsoft\Windows NT\CurrentVersion)", "InstallDate");
  const auto install_time = std::chrono::system_clock::from_time_t(since_install);

  return {cur_time, install_time, boot_time};
}
#elif defined(MAC)
#include "../shared/ns.h"

cspec::system::times_t cspec::system::times()
{
  struct timeval tv
  {
  };
  const auto cur_time = std::chrono::system_clock::now();
  cspec::shared::sysctlstruct<struct timeval>("kern.boottime", tv);
  const auto uptime = std::chrono::seconds(tv.tv_sec);
  const auto boot_time = std::chrono::system_clock::time_point(uptime);

  const auto raw_update = cspec::shared::exec(
    R"(system_profiler SPInstallHistoryDataType | grep -A4 -E '^\s{4}(macOS|OS X)' | tail -n1 | awk '{ print $3 $4; }')");
  const auto update_time = cspec::shared::stotm(raw_update, "%Y-%m-%d,%H:%M");

  return {cur_time, update_time, boot_time};
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

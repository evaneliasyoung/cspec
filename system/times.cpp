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

#if defined(WIN)
#include <windows.h>

cspec::system::times_t cspec::system::times()
{
  const auto cur_time = std::chrono::system_clock::now();
  const auto uptime = std::chrono::milliseconds(GetTickCount64());
  const auto boot_time = cur_time - uptime;
  return {cur_time, boot_time};
}
#elif defined(MAC)
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

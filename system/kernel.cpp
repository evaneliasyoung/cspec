/**
 *  @file      kernel.cpp
 *  @brief     Determines system kernel.
 *
 *  @author    Evan Elias Young
 *  @date      2021-11-17
 *  @date      2021-11-20
 *  @copyright Copyright 2021 Evan Elias Young. All rights reserved.
 */

#include "ns.h"

cspec::system::kernel_t cspec::system::string_to_kernel(const string &kernel)
{
  if (strcasecmp(kernel.c_str(), "Windows NT") == 0)
    return cspec::system::kernel_t::nt;
  else if (strcasecmp(kernel.c_str(), "Linux") == 0)
    return cspec::system::kernel_t::linux;
  else if (strcasecmp(kernel.c_str(), "Darwin") == 0)
    return cspec::system::kernel_t::darwin;
  else
    return cspec::system::kernel_t::unknown;
}

string cspec::system::kernel_to_string(const cspec::system::kernel_t &kernel)
{
  switch (kernel)
  {
    case cspec::system::kernel_t::nt:
      return "Windows NT";
    case cspec::system::kernel_t::linux:
      return "Linux";
    case cspec::system::kernel_t::darwin:
      return "Darwin";
    default:
      return "Unknown";
  }
}

#if defined(WIN)
cspec::system::kernel_info_t cspec::system::kernel()
{
  string path;
  path.resize(GetSystemDirectoryA(nullptr, 0) - 1);
  GetSystemDirectoryA(&path[0], static_cast<UINT>(path.size() + 1));
  path += "\\kernel32.dll";

  const auto ver_info_len = GetFileVersionInfoSizeA(path.c_str(), nullptr);
  auto ver_info = std::make_unique<std::uint8_t[]>(ver_info_len);
  GetFileVersionInfoA(path.c_str(), 0, ver_info_len, ver_info.get());

  VS_FIXEDFILEINFO *file_version;
  unsigned int file_version_len;
  VerQueryValueA(ver_info.get(), "", reinterpret_cast<void **>(&file_version), &file_version_len);

  return {cspec::system::kernel_t::nt, HIWORD(file_version->dwProductVersionMS),
          LOWORD(file_version->dwProductVersionMS), HIWORD(file_version->dwProductVersionLS),
          LOWORD(file_version->dwProductVersionLS)};
}
#else
#include <sys/utsname.h>

cspec::system::kernel_info_t cspec::system::kernel()
{
  utsname uts;
  uname(&uts);

  char *marker = uts.release;
  const u32 major = std::strtoul(marker, &marker, 10);
  const u32 minor = std::strtoul(marker + 1, &marker, 10);
  const u32 patch = std::strtoul(marker + 1, &marker, 10);
  const u32 build = std::strtoul(marker + 1, nullptr, 10);

  const auto kernel = string_to_kernel(uts.sysname);

  return {kernel, major, minor, patch, build};
}
#endif

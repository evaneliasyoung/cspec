/**
 *  @file      os.cpp
 *  @brief     Determines system os.
 *
 *  @author    Evan Elias Young
 *  @date      2021-11-17
 *  @date      2021-11-17
 *  @copyright Copyright 2021 Evan Elias Young. All rights reserved.
 */

#include "ns.h"
#include "utils.hpp"

#if defined(WIN)
cspec::system::os_info_t cspec::system::os()
{
  const auto kernel_version = cspec::system::kernel_info();
  return {"Windows NT",         version_name(),       kernel_version.major,
          kernel_version.minor, kernel_version.patch, kernel_version.build};
}
#else
cspec::system::os_info_t cspec::system::os()
{
  std::ifstream release("/etc/os-release");

  if (!release.is_open() || !release)
  {
    release.open("/usr/lib/os-release", std::ios::in);
    if (!release.is_open() || !release)
    {
      release.open("/etc/lsb-release", std::ios::in);
      if (!release.is_open() || !release)
        return {};
      else
        return lsb_release(release);
    }
  }

  cspec::system::os_info_t ret{};

  for (std::string line; std::getline(release, line);)
  {
    if (line.find("NAME") == 0)
      ret.name = line.substr(line.find('=') + 1);
    else if (line.find("PRETTY_NAME") == 0)
      ret.full_name = line.substr(line.find('=') + 1);
    else if (line.find("VERSION_ID") == 0)
    {
      char *marker = &line[line.find('=') + 1];
      if (marker[0] == '"')
        ++marker;
      ret.major = std::strtoul(marker, &marker, 10);
      if (marker[0] && marker[0] != '"')
        ret.minor = std::strtoul(marker + 1, &marker, 10);
      if (marker[0] && marker[0] != '"')
        ret.patch = std::strtoul(marker + 1, &marker, 10);
      if (marker[0] && marker[0] != '"')
        ret.build = std::strtoul(marker + 1, nullptr, 10);
    }
  }

  trim_quotes(ret.name);
  trim_quotes(ret.full_name);

  return ret;
}
#endif

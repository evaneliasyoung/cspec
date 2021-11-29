/**
 *  @file      os.cpp
 *  @brief     Determines system os.
 *
 *  @author    Evan Elias Young
 *  @date      2021-11-17
 *  @date      2021-11-29
 *  @copyright Copyright 2021 Evan Elias Young. All rights reserved.
 */

#include "ns.h"

void cspec::system::to_json(json &j, const cspec::system::os_info_t &os)
{
  j = json{{"name", os.name}, {"full_name", os.full_name}, {"version", os.version}};
}

void cspec::system::from_json(const json &j, cspec::system::os_info_t &os)
{
  j.at("name").get_to(os.name);
  j.at("full_name").get_to(os.full_name);
  j.at("version").get_to(os.version);
}

#if defined(WIN)
#include "../utils/win/wmi.hpp"

static string version_name()
{
  WMI wmi;
  if (!wmi.prepare())
    return "";
  const auto full_name = wmi.query_and_retrieve<string>("Win32_OperatingSystem", {"Name"}).at("Name");
  return full_name.substr(0, full_name.find("|"));
}

cspec::system::os_info_t cspec::system::os()
{
  const auto version = cspec::system::kernel().version;
  return {"Windows NT", version_name(), version};
}
#elif defined(MAC)
#include "../utils/mac/sysctl.hpp"

cspec::system::os_info_t cspec::system::os()
{
  cspec::shared::version_t ver{};
  string buf;
  string name;
  string full_name;

  if (sysctlstring<16>("kern.osrelease", buf))
  {
    umax kern = std::stoull(buf.substr(0, buf.find_first_of('.'))) - 4;
    array ver_names = {"Cheetah",      "Puma",        "Jaguar",        "Panther",   "Tiger",    "Leopard",
                       "Snow Leopard", "Lion",        "Mountain Lion", "Mavericks", "Yosemite", "El Capitan",
                       "Sierra",       "High Sierra", "Mojave",        "Catalina",  "Big Sur",  "Monterey"};
    name = kern < 16 ? "Mac OS X" : "macOS";
    full_name = name + " " + string(kern < ver_names.size() ? ver_names[kern] : "Unknown");
  }

  if (sysctlstring<16>("kern.osproductversion", buf))
  {
    std::smatch mt;
    if (std::regex_search(buf, mt, R"(^(\d+)(?:\.(\d+))?(?:\.(\d+))?$)"_regex))
    {
      if (!mt[1].str().empty())
        ver.major = std::stoull(mt[1]);
      if (!mt[2].str().empty())
        ver.minor = std::stoull(mt[2]);
      if (!mt[3].str().empty())
        ver.patch = std::stoull(mt[3]);
    }
  }
  if (sysctlstring<16>("kern.osversion", buf))
    ver.build = std::stoull(buf, 0, 16);

  return {name, full_name, ver};
}
#else
#include <fstream>

static cspec::system::os_info_t lsb_release(std::ifstream &release)
{
  cspec::system::os_info_t ret{};

  for (string line; std::getline(release, line);)
  {
    if (line.find("DISTRIB_ID") == 0)
      ret.name = line.substr(line.find('=') + 1);
    else if (line.find("DISTRIB_RELEASE") == 0)
    {
      char *marker = &line[line.find('=') + 1];
      ret.version.major = std::strtoul(marker, &marker, 10);
      ret.version.minor = std::strtoul(marker + 1, &marker, 10);
      ret.version.patch = std::strtoul(marker + 1, &marker, 10);
      ret.version.build = std::strtoul(marker + 1, nullptr, 10);
    }
    else if (line.find("DISTRIB_DESCRIPTION") == 0)
    {
      const auto beg_idx = line.find('"') + 1;
      const auto end_idx = line.size() - 1;
      ret.full_name = line.substr(beg_idx, end_idx - beg_idx);
    }
  }

  return ret;
}

static void trim_quotes(string &s)
{
  if (s.empty())
    return;
  if (s.back() == '"')
    s.pop_back();
  if (s.front() == '"')
    s.erase(s.begin());
}

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

  for (string line; std::getline(release, line);)
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
      ret.version.major = std::strtoul(marker, &marker, 10);
      if (marker[0] && marker[0] != '"')
        ret.version.minor = std::strtoul(marker + 1, &marker, 10);
      if (marker[0] && marker[0] != '"')
        ret.version.patch = std::strtoul(marker + 1, &marker, 10);
      if (marker[0] && marker[0] != '"')
        ret.version.build = std::strtoul(marker + 1, nullptr, 10);
    }
  }

  trim_quotes(ret.name);
  trim_quotes(ret.full_name);

  return ret;
}
#endif

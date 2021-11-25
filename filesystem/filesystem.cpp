/**
 *  @file      filesystems.cpp
 *  @brief     Determines system filesystems.
 *
 *  @author    Evan Elias Young
 *  @date      2021-11-23
 *  @date      2021-11-24
 *  @copyright Copyright 2021 Evan Elias Young. All rights reserved.
 */

#include "ns.h"

cspec::filesystem::filesystem_type_t cspec::filesystem::stofs(const string &filesystem)
{
  if (strcasecmp(filesystem.c_str(), "NTFS") == 0)
    return cspec::filesystem::filesystem_type_t::ntfs;
  else if (strcasecmp(filesystem.c_str(), "FAT") == 0)
    return cspec::filesystem::filesystem_type_t::fat;
  else if (strcasecmp(filesystem.c_str(), "exFAT") == 0)
    return cspec::filesystem::filesystem_type_t::exfat;
  else if (strcasecmp(filesystem.c_str(), "HFS") == 0)
    return cspec::filesystem::filesystem_type_t::hfs;
  else if (strcasecmp(filesystem.c_str(), "EXT") == 0)
    return cspec::filesystem::filesystem_type_t::ext;
  else
    return cspec::filesystem::filesystem_type_t::unknown;
}

string cspec::filesystem::fstos(const cspec::filesystem::filesystem_type_t &filesystem)
{
  switch (filesystem)
  {
    case cspec::filesystem::filesystem_type_t::ntfs:
      return "NTFS";
    case cspec::filesystem::filesystem_type_t::fat:
      return "FAT";
    case cspec::filesystem::filesystem_type_t::exfat:
      return "exFAT";
    case cspec::filesystem::filesystem_type_t::hfs:
      return "HFS";
    case cspec::filesystem::filesystem_type_t::ext:
      return "EXT";
    case cspec::filesystem::filesystem_type_t::afps:
      return "AFPS";
    default:
      return "Unknown";
  }
}

void cspec::filesystem::to_json(json &j, const cspec::filesystem::sizes_t &sizes)
{
  j = json{{"available", sizes.available}, {"total", sizes.total}, {"used", sizes.used}};
}

void cspec::filesystem::from_json(const json &j, cspec::filesystem::sizes_t &sizes)
{
  j.at("available").get_to(sizes.available);
  j.at("total").get_to(sizes.total);
  j.at("used").get_to(sizes.used);
}

void cspec::filesystem::to_json(json &j, const cspec::filesystem::filesystem_t &fs)
{
  j = json{{"name", fs.name}, {"type", cspec::filesystem::fstos(fs.type)}, {"sizes", fs.sizes}, {"mount", fs.mount}};
}

void cspec::filesystem::from_json(const json &j, cspec::filesystem::filesystem_t &fs)
{
  j.at("name").get_to(fs.name);
  j.at("sizes").get_to(fs.sizes);
  j.at("mount").get_to(fs.mount);
  fs.type = cspec::filesystem::stofs(j.at("type"));
}

#if defined(WIN)
#include "../utils/win/wmi.hpp"

vector<cspec::filesystem::filesystem_t> cspec::filesystem::systems()
{
  vector<cspec::filesystem::filesystem_t> ret{};

  WMI wmi;
  if (!wmi.prepare())
    return ret;

  const auto filesystems =
    wmi.query_and_retrieve_multiple<string>("Win32_LogicalDisk", {"Caption", "FileSystem", "FreeSpace", "Size"});

  for (const auto fs: filesystems)
  {
    cspec::filesystem::filesystem_t filesystem;

    filesystem.name = fs.at("Caption");
    filesystem.type = stofs(fs.at("FileSystem"));
    filesystem.mount = fs.at("Caption");
    filesystem.sizes.total = !fs.at("Size").empty() ? std::stoull(fs.at("Size")) : 0;
    filesystem.sizes.available = !fs.at("FreeSpace").empty() ? std::stoull(fs.at("FreeSpace")) : 0;
    filesystem.sizes.used = filesystem.sizes.total - filesystem.sizes.available;

    ret.push_back(filesystem);
  }

  return ret;
}
#elif defined(MAC)
#else
#endif

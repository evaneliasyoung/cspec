/**
 *  @file      filesystems.cpp
 *  @brief     Determines system filesystems.
 *
 *  @author    Evan Elias Young
 *  @date      2021-11-23
 *  @date      2021-11-23
 *  @copyright Copyright 2021 Evan Elias Young. All rights reserved.
 */

#include "ns.h"

#if defined(WIN)
#include "../utils/win/wmi.hpp"

cspec::filesystem::filesystem_type_t cspec::filesystem::string_to_filesystem(const string &filesystem)
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

string cspec::filesystem::filesystem_type_to_string(const cspec::filesystem::filesystem_type_t &filesystem)
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
    filesystem.type = string_to_filesystem(fs.at("FileSystem"));
    filesystem.mount = fs.at("Caption");
    filesystem.size = !fs.at("Size").empty() ? std::stoull(fs.at("Size")) : 0;
    filesystem.available = !fs.at("FreeSpace").empty() ? std::stoull(fs.at("FreeSpace")) : 0;
    filesystem.used = filesystem.size - filesystem.available;

    ret.push_back(filesystem);
  }

  return ret;
}
#elif defined(MAC)
#else
#endif

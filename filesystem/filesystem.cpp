/**
 *  @file      filesystems.cpp
 *  @brief     Determines system filesystems.
 *
 *  @author    Evan Elias Young
 *  @date      2021-11-23
 *  @date      2021-11-29
 *  @copyright Copyright 2021 Evan Elias Young. All rights reserved.
 */

#include "ns.h"

#if defined(WIN)
#include "../shared/ns.h"

vector<cspec::filesystem::filesystem_t> cspec::filesystem::systems()
{
  vector<cspec::filesystem::filesystem_t> ret{};

  cspec::shared::WMI wmi;
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
    filesystem.sizes.total = !fs.at("Size").empty() ? cspec::shared::depow2(std::stoull(fs.at("Size"))) : 0;
    filesystem.sizes.available =
      !fs.at("FreeSpace").empty() ? cspec::shared::depow2(std::stoull(fs.at("FreeSpace"))) : 0;
    filesystem.sizes.used = filesystem.sizes.total - filesystem.sizes.available;

    ret.push_back(filesystem);
  }

  return ret;
}
#elif defined(MAC)
#include <iostream>
#include <sys/mount.h>

vector<cspec::filesystem::filesystem_t> cspec::filesystem::systems()
{
  vector<cspec::filesystem::filesystem_t> ret{};

  struct statfs *mounts;
  umax num = getmntinfo(&mounts, MNT_WAIT);
  if (num < 0)
    return ret;

  for (auto i = 0; i < num; ++i)
    if (mounts[i].f_blocks > 0)
    {
      cspec::filesystem::filesystem_t fs;
      const auto mt = mounts[i];
      const auto bsize = mt.f_bsize;
      fs.type = cspec::filesystem::stofs(mt.f_fstypename);
      fs.mount = mt.f_mntonname;
      if (fs.mount.find_last_of('/') != string::npos)
        fs.name = fs.mount.substr(fs.mount.find_last_of('/') + 1);
      fs.sizes.total = mt.f_blocks * bsize;
      fs.sizes.available = mt.f_bavail * bsize;
      fs.sizes.used = fs.sizes.total - fs.sizes.available;
      ret.push_back(fs);
    }

  return ret;
}
#else
#include <filesystem>
#include <fstream>
#include <map>

vector<cspec::filesystem::filesystem_t> cspec::filesystem::systems()
{
  vector<cspec::filesystem::filesystem_t> ret{};

  std::ifstream proc_mounts("/proc/mounts");
  if (!proc_mounts.is_open() || !proc_mounts)
    return ret;

  for (string line; std::getline(proc_mounts, line);)
    if (line.size() > 0 && line[0] == '/')
    {
      cspec::filesystem::filesystem_t fs;
      const auto name_space = line.find_first_of(' ');
      const auto mount_space = line.find_first_of(' ', name_space + 1);
      const auto type_space = line.find_first_of(' ', mount_space + 1);
      fs.name = line.substr(0, name_space);
      fs.mount = line.substr(name_space + 1, mount_space - name_space - 1);
      fs.type = cspec::filesystem::stofs(line.substr(mount_space + 1, type_space - mount_space - 1));

      const auto si = std::filesystem::space(fs.mount);
      fs.sizes.total = si.capacity;
      fs.sizes.available = si.available;
      fs.sizes.used = fs.sizes.total - fs.sizes.available;
      ret.push_back(fs);
    };

  return ret;
}
#endif

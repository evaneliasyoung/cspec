/**
 *  @file      ns.h
 *  @brief     Namspace extensions for cspec filesystems.
 *
 *  @author    Evan Elias Young
 *  @date      2021-11-23
 *  @date      2021-11-29
 *  @copyright Copyright 2021 Evan Elias Young. All rights reserved.
 */

#pragma once

#include "../core.h"

namespace cspec
{
  namespace filesystem
  {
    enum class filesystem_type_t
    {
      ntfs,
      fat,
      exfat,
      hfs,
      ext,
      ext4,
      apfs,
      devfs,
      unknown
    };
    filesystem_type_t stofs(const string &filesystem);
    string fstos(const filesystem_type_t &filesystem);

    struct sizes_t
    {
      umax total;
      umax used;
      umax available;
    };
    void to_json(json &j, const sizes_t &sizes);
    void from_json(const json &j, sizes_t &sizes);

    struct filesystem_t
    {
      string name;
      filesystem_type_t type;
      sizes_t sizes;
      string mount;
    };
    void to_json(json &j, const filesystem_t &fs);
    void from_json(const json &j, filesystem_t &fs);

    vector<filesystem_t> systems();
    json collect();
    json collect(const vector<string> &keys);
  } // namespace filesystem

} // namespace cspec

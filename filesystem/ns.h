/**
 *  @file      ns.h
 *  @brief     Namspace extensions for cspec filesystems.
 *
 *  @author    Evan Elias Young
 *  @date      2021-11-23
 *  @date      2021-11-23
 *  @copyright Copyright 2021 Evan Elias Young. All rights reserved.
 */

#pragma once

#include "../core.h"

#include <vector>

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
      afps,
      unknown
    };

    struct filesystem_t
    {
      string name;
      filesystem_type_t type;
      umax size;
      umax used;
      umax available;
      string mount;
    };

    vector<filesystem_t> systems();
    filesystem_type_t string_to_filesystem(const string &filesystem);
    string filesystem_type_to_string(const filesystem_type_t &filesystem);
  } // namespace filesystem

} // namespace cspec

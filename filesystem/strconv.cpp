/**
 *  @file      strconv.cpp
 *  @brief     Handles filesystem related string conversions.
 *
 *  @author    Evan Elias Young
 *  @date      2021-11-29
 *  @date      2021-11-29
 *  @copyright Copyright 2021 Evan Elias Young. All rights reserved.
 */

#include "../shared/ns.h"
#include "ns.h"

cspec::filesystem::filesystem_type_t cspec::filesystem::stofs(const string &filesystem)
{
  if (cspec::shared::icaseis(filesystem, "NTFS"))
    return cspec::filesystem::filesystem_type_t::ntfs;
  else if (cspec::shared::icaseis(filesystem, "FAT"))
    return cspec::filesystem::filesystem_type_t::fat;
  else if (cspec::shared::icaseis(filesystem, "exFAT"))
    return cspec::filesystem::filesystem_type_t::exfat;
  else if (cspec::shared::icaseis(filesystem, "HFS"))
    return cspec::filesystem::filesystem_type_t::hfs;
  else if (cspec::shared::icaseis(filesystem, "EXT"))
    return cspec::filesystem::filesystem_type_t::ext;
  else if (cspec::shared::icaseis(filesystem, "EXT4"))
    return cspec::filesystem::filesystem_type_t::ext4;
  else if (cspec::shared::icaseis(filesystem, "APFS"))
    return cspec::filesystem::filesystem_type_t::apfs;
  else if (cspec::shared::icaseis(filesystem, "DevFS"))
    return cspec::filesystem::filesystem_type_t::devfs;
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
    case cspec::filesystem::filesystem_type_t::ext4:
      return "EXT4";
    case cspec::filesystem::filesystem_type_t::apfs:
      return "APFS";
    case cspec::filesystem::filesystem_type_t::devfs:
      return "DevFS";
    default:
      return "Unknown";
  }
}

/**
 *  @file      registry.cpp
 *  @brief     Windows registry methods.
 *
 *  @author    Evan Elias Young
 *  @date      2021-11-29
 *  @date      2021-11-29
 *  @copyright Copyright 2021 Evan Elias Young. All rights reserved.
 */

#include "ns.h"

#ifdef WIN
bool cspec::shared::open_registry_path(HKEY *hkey, HKEY source_key, const char *path)
{
  if (RegOpenKeyExA(source_key, path, 0, KEY_READ, hkey))
    return false;
  return true;
}

u32 cspec::shared::read_registry_dw(HKEY source_key, const char *path, const char *key)
{
  HKEY hkey;
  if (!cspec::shared::open_registry_path(&hkey, source_key, path))
    return {};

  u32 identifier;
  DWORD identifier_len = sizeof(identifier);
  LPBYTE lpdata = static_cast<LPBYTE>(static_cast<void *>(&identifier));
  if (RegQueryValueExA(hkey, key, nullptr, nullptr, lpdata, &identifier_len))
    return {};

  return identifier;
}

umax cspec::shared::read_registry_qw(HKEY source_key, const char *path, const char *key)
{
  HKEY hkey;
  if (RegOpenKeyExA(source_key, path, 0, KEY_READ, &hkey))
    return {};

  umax identifier;
  DWORD identifier_len = sizeof(identifier);
  LPBYTE lpdata = static_cast<LPBYTE>(static_cast<void *>(&identifier));
  if (RegQueryValueExA(hkey, key, nullptr, nullptr, lpdata, &identifier_len))
    return {};

  return identifier;
}
#endif

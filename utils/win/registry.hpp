/**
 *  @file      registry.hpp
 *  @brief     Windows registry methods.
 *
 *  @author    Evan Elias Young
 *  @date      2021-11-19
 *  @date      2021-11-19
 *  @copyright Copyright 2021 Evan Elias Young. All rights reserved.
 */

#pragma once

#include "../../pch.h"

#ifdef WIN
static bool open_registry_path(HKEY *hkey, HKEY source_key, const char *path)
{
  if (RegOpenKeyExA(source_key, path, 0, KEY_READ, hkey))
    return false;
  return true;
}

static u32 read_registry_dw(HKEY source_key, const char *path, const char *key)
{
  HKEY hkey;
  if (!open_registry_path(&hkey, source_key, path))
    return {};

  u32 identifier;
  DWORD identifier_len = sizeof(identifier);
  LPBYTE lpdata = static_cast<LPBYTE>(static_cast<void *>(&identifier));
  if (RegQueryValueExA(hkey, key, nullptr, nullptr, lpdata, &identifier_len))
    return {};

  return identifier;
}

static umax read_registry_qw(HKEY source_key, const char *path, const char *key)
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

template<u32 BufferSize> static std::string read_registry_sz(HKEY source_key, const char *path, const char *key)
{
  HKEY hkey;
  if (RegOpenKeyExA(source_key, path, 0, KEY_READ, &hkey))
    return {};

  char identifier[BufferSize];
  DWORD identifier_len = sizeof(identifier);
  LPBYTE lpdata = static_cast<LPBYTE>(static_cast<void *>(&identifier[0]));
  if (RegQueryValueExA(hkey, key, nullptr, nullptr, lpdata, &identifier_len))
    return {};

  return identifier;
}
#endif

/**
 *  @file      registry.tpp
 *  @brief     Templated Windows registry methods.
 *
 *  @author    Evan Elias Young
 *  @date      2021-11-29
 *  @date      2021-11-29
 *  @copyright Copyright 2021 Evan Elias Young. All rights reserved.
 */

#ifdef WIN
template<u32 BufferSize> string cspec::shared::read_registry_sz(HKEY source_key, const char *path, const char *key)
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

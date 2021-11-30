/**
 *  @file      sysctl.tpp
 *  @brief     System control access wrappers.
 *
 *  @author    Evan Elias Young
 *  @date      2021-11-29
 *  @date      2021-11-29
 *  @copyright Copyright 2021 Evan Elias Young. All rights reserved.
 */

#pragma once

#include <sys/sysctl.h>

template<typename T, typename S, size_t BufferLength> bool cspec::shared::basesysctl(const char *path, T &out)
{
  size_t len = sizeof(S) * BufferLength;
  return sysctlbyname(path, &out, &len, NULL, 0) == 0;
}

template<size_t BufferLength> bool cspec::shared::sysctlstring(const char *path, string &out)
{
  array<char, BufferLength> buf;
  if (!cspec::shared::basesysctl<array<char, BufferLength>, char, BufferLength>(path, buf))
    return false;

  size_t real_len = 0;
  while (buf[real_len] != 0)
    ++real_len;
  out = string(buf.begin(), real_len);

  return true;
}

template<typename Integral> bool cspec::shared::sysctlintegral(const char *path, Integral &out)
{
  return cspec::shared::basesysctl<Integral, Integral, 1>(path, out);
}

template<typename T, size_t Size> bool cspec::shared::sysctlarray(const char *path, array<T, Size> &out)
{
  return cspec::shared::basesysctl<array<T, Size>, T, Size>(path, out);
}

template<class T> bool cspec::shared::sysctlstruct(const char *path, T &out)
{
  return cspec::shared::basesysctl<T, T, 1>(path, out);
}

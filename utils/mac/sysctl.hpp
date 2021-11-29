/**
 *  @file      sysctl.hpp
 *  @brief     System control access wrappers.
 *
 *  @author    Evan Elias Young
 *  @date      2021-11-29
 *  @date      2021-11-29
 *  @copyright Copyright 2021 Evan Elias Young. All rights reserved.
 */

#pragma once

#include "../../core.h"

#include <sys/sysctl.h>

template<typename T, typename S = T, size_t BufferLength = 1> bool basesysctl(const char *path, T &out)
{
  size_t len = sizeof(S) * BufferLength;
  return sysctlbyname(path, &out, &len, NULL, 0) == 0;
}

template<size_t BufferLength = 255> bool sysctlstring(const char *path, string &out)
{
  array<char, BufferLength> buf;
  if (!basesysctl<array<char, BufferLength>, char, BufferLength>(path, buf))
    return false;

  size_t real_len = 0;
  while (buf[real_len] != 0)
    ++real_len;
  out = string(buf.begin(), real_len);

  return true;
}

template<typename Integral = u32> bool sysctlintegral(const char *path, Integral &out)
{
  return basesysctl<Integral, Integral, 1>(path, out);
}

template<typename T, size_t Size> bool sysctlarray(const char *path, array<T, Size> &out)
{
  return basesysctl<array<T, Size>, T, Size>(path, out);
}

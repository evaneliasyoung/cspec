/**
 *  @file      ns.h
 *  @brief     Shared code for macOS.
 *
 *  @author    Evan Elias Young
 *  @date      2021-11-29
 *  @date      2021-11-29
 *  @copyright Copyright 2021 Evan Elias Young. All rights reserved.
 */

#pragma once

#include "../../core.h"

namespace cspec
{
  namespace shared
  {
    template<typename T, typename S = T, size_t BufferLength = 1> bool basesysctl(const char *path, T &out);
    template<size_t BufferLength = 255> bool sysctlstring(const char *path, string &out);
    template<typename Integral = u32> bool sysctlintegral(const char *path, Integral &out);
    template<typename T, size_t Size> bool sysctlarray(const char *path, array<T, Size> &out);
    template<class T> bool sysctlstruct(const char *path, T &out);
  } // namespace shared
} // namespace cspec

#include "sysctl.tpp"

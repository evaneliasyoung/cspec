/**
 *  @file      pch.h
 *  @brief     Precompiled project headers.
 *
 *  @author    Evan Elias Young
 *  @date      2021-11-11
 *  @date      2021-11-16
 *  @copyright Copyright 2021 Evan Elias Young. All rights reserved.
 */

#pragma once

#if defined(_WIN32) || defined(_WIN64)
#define WIN
#endif
#if defined(__APPLE__) && defined(__MACH__)
#define MAC
#endif
#if defined(__GNU__) || defined(__gnu_linux__)
#define TUX
#endif

#include <algorithm>
#include <bitset>
#include <cstring>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <unistd.h>
#include <vector>

#ifdef WIN
#include <windows.h>
#endif

#ifdef MAC
#endif

#ifdef TUX
#include <sys/sysinfo.h>
#include <sys/utsname.h>
#endif

using u8 = std::uint8_t;
using u16 = std::uint16_t;
using u32 = std::uint32_t;
using u64 = std::uint64_t;
using umax = std::uintmax_t;
using uptr = std::uintptr_t;
using s8 = std::int8_t;
using s16 = std::int16_t;
using s32 = std::int32_t;
using s64 = std::int64_t;
using smax = std::intmax_t;
using sptr = std::intptr_t;

/**
 *  @file      core.h
 *  @brief     Core headers and definitions.
 *
 *  @author    Evan Elias Young
 *  @date      2021-11-20
 *  @date      2021-11-24
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

#include "lib/json/single_include/nlohmann/json.hpp"

#include <array>
#include <cstdint>
#include <cstring>
#include <regex>
#include <string>
#include <vector>

using u8 = std::uint8_t;
using u16 = std::uint16_t;
using u32 = std::uint32_t;
using u64 = std::uint64_t;
using umax = std::uintmax_t;
using uptr = std::uintptr_t;
using nlohmann::json;
using std::array;
using std::string;
using std::vector;
using std::wstring;

inline std::regex operator"" _regex(const char *s, umax n)
{
  return std::regex(s, n, std::regex_constants::ECMAScript);
}

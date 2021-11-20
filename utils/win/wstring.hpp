/**
 *  @file      wstring.hpp
 *  @brief     Handles narrow/enlarge w and bstring.
 *
 *  @author    Evan Elias Young
 *  @date      2021-11-19
 *  @date      2021-11-19
 *  @copyright Copyright 2021 Evan Elias Young. All rights reserved.
 */

#pragma once

#include "../pch.h"

#ifdef WIN
static std::string transcode_from_wide(const wchar_t *wstr, std::size_t wstr_size)
{
  std::string ret;
  // convert even embedded NUL
  if (const auto len = WideCharToMultiByte(CP_UTF8, 0, wstr, static_cast<int>(wstr_size), nullptr, 0, 0, 0))
  {
    ret.resize(len, '\0');
    if (!WideCharToMultiByte(CP_UTF8, 0, wstr, static_cast<int>(wstr_size), &ret[0], len, 0, 0))
      ret.clear();
  }
  return ret;
}

std::string narrow_wstring(const wchar_t *wstr)
{
  if (!wstr)
    return {};
  return transcode_from_wide(wstr, std::wcslen(wstr));
}

std::string narrow_bstring(const wchar_t *bstr)
{
  if (!bstr)
    return {};
  return transcode_from_wide(bstr, SysStringLen(const_cast<BSTR>(bstr)));
}
#endif

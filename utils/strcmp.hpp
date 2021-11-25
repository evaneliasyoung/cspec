/**
 *  @file      strcmp.hpp
 *  @brief     String comparison and searching methods.
 *
 *  @author    Evan Elias Young
 *  @date      2021-11-25
 *  @date      2021-11-25
 *  @copyright Copyright 2021 Evan Elias Young. All rights reserved.
 */

#pragma once

#include "../core.h"

#include <algorithm>

inline bool icaseis(const char *&a, const char *&b)
{
  return strcasecmp(a, b) == 0;
}

inline bool icaseis(const string &a, const string &b)
{
  return icaseis(a.c_str(), b.c_str());
}

inline bool icaseis(const string &a, const char *&b)
{
  return icaseis(a.c_str(), b);
}

inline bool icaseis(const char *&a, const string &b)
{
  return icaseis(a, b.c_str());
}

inline bool contains_icase(const vector<string> &v, const string &s)
{
  return std::find_if(v.cbegin(), v.cend(),
                      [&s](const string &x) -> bool
                      {
                        return icaseis(s, x);
                      }) != std::end(v);
}

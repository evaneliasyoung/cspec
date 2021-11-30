/**
 *  @file      strcase.cpp
 *  @brief     String comparison and searching methods.
 *
 *  @author    Evan Elias Young
 *  @date      2021-11-29
 *  @date      2021-11-29
 *  @copyright Copyright 2021 Evan Elias Young. All rights reserved.
 */

#include "ns.h"

#include <algorithm>
#include <iostream>

string cspec::shared::to_lower(const string &s)
{
  string ret{};
  std::transform(s.begin(), s.end(), ret.begin(),
                 [](auto c)
                 {
                   return std::tolower(c);
                 });
  return ret;
}

string cspec::shared::to_lower(string &s)
{
  std::transform(s.begin(), s.end(), s.begin(),
                 [](auto c)
                 {
                   return std::tolower(c);
                 });
  return s;
}

bool cspec::shared::icaseis(const char *&a, const char *&b)
{
  return strcasecmp(a, b) == 0;
}

bool cspec::shared::icaseis(const string &a, const string &b)
{
  return strcasecmp(a.c_str(), b.c_str()) == 0;
}

bool cspec::shared::icaseis(const string &a, const char *&b)
{
  return strcasecmp(a.c_str(), b) == 0;
}

bool cspec::shared::icaseis(const char *&a, const string &b)
{
  return strcasecmp(a, b.c_str()) == 0;
}

bool cspec::shared::contains_icase(const vector<string> &v, const string &s)
{
  return std::find_if(v.cbegin(), v.cend(),
                      [&s](const string &x) -> bool
                      {
                        return icaseis(s, x);
                      })
         != std::end(v);
}

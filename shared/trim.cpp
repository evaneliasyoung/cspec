/**
 *  @file      trim.cpp
 *  @brief     Handles trimming strings.
 *
 *  @author    Evan Elias Young
 *  @date      2021-11-29
 *  @date      2021-11-29
 *  @copyright Copyright 2021 Evan Elias Young. All rights reserved.
 */

#include "ns.h"

string cspec::shared::rtrim(string s, const char *t)
{
  return s.erase(s.find_last_not_of(t) + 1);
}

string cspec::shared::ltrim(string s, const char *t)
{
  return s.erase(0, s.find_first_not_of(t));
}

string cspec::shared::trim(string s, const char *t)
{
  return ltrim(rtrim(s, t), t);
}

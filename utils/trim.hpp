/**
 *  @file      trim.hpp
 *  @brief     Handles trimming strings.
 *
 *  @author    Evan Elias Young
 *  @date      2021-11-24
 *  @date      2021-11-24
 *  @copyright Copyright 2021 Evan Elias Young. All rights reserved.
 */

#pragma once

#include "../core.h"

string rtrim(string s, const char *t = " \t\n\r\f\v")
{
  return s.erase(s.find_last_not_of(t) + 1);
}

string ltrim(string s, const char *t = " \t\n\r\f\v")
{
  return s.erase(0, s.find_first_not_of(t));
}

string trim(string s, const char *t = " \t\n\r\f\v")
{
  return ltrim(rtrim(s, t), t);
}

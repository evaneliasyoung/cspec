/**
 *  @file      ns.h
 *  @brief     Shared structures and methods in cspec.
 *
 *  @author    Evan Elias Young
 *  @date      2021-11-24
 *  @date      2021-11-29
 *  @copyright Copyright 2021 Evan Elias Young. All rights reserved.
 */

#pragma once

#include "../core.h"

#include <sstream>

#if defined(WIN)
#elif defined(MAC)
#include "mac/ns.h"
#else
#endif

namespace cspec
{
  namespace shared
  {
    struct version_t
    {
      u64 major;
      u64 minor;
      u64 patch;
      u64 build;
    };
    string vertos(const version_t &ver);
    version_t stover(const string &ver);
    void to_json(json &j, const version_t &ver);
    void from_json(const json &j, version_t &ver);

    string exec(const char *cmd);
    umax depow2(const umax &bi);

    string rtrim(string s, const char *t = " \t\n\r\f\v");
    string ltrim(string s, const char *t = " \t\n\r\f\v");
    string trim(string s, const char *t = " \t\n\r\f\v");

    string to_lower(const string &s);
    string to_lower(string &s);
    bool icaseis(const char *&a, const char *&b);
    bool icaseis(const string &a, const string &b);
    bool icaseis(const string &a, const char *&b);
    bool icaseis(const char *&a, const string &b);
    bool contains_icase(const vector<string> &v, const string &s);
  } // namespace shared
} // namespace cspec

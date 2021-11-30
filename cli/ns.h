/**
 *  @file      cli.h
 *  @brief     Command line interface for cspec.
 *
 *  @author    Evan Elias Young
 *  @date      2021-11-25
 *  @date      2021-11-29
 *  @copyright Copyright 2021 Evan Elias Young. All rights reserved.
 */

#pragma once

#include "../core.h"
#include "../lib/argparse/include/argparse/argparse.hpp"
#include "../shared/ns.h"

#include <iostream>
#include <map>

namespace cspec
{
  namespace cli
  {
    const std::map<string, vector<string>> queries = {
      {"cpu", {"name", "amounts", "architecture", "clock", "cache", "endian", "vendor", "group"}},
      {"filesystem", {"name", "sizes", "mount", "type"}},
      {"gpu", {"name", "memory", "vendor", "bus", "dynamic"}},
      {"memory", {"voltage", "form_factor", "size", "speed", "manufacturer", "model", "serial", "bank"}},
      {"system", {"os", "kernel", "times"}}};
    const vector<string> namespaces = {"cpu", "filesystem", "gpu", "memory", "system"};

    class exception
    {
      protected:
      string _msg;

      public:
      const char *what() const;
    };

    class invalid_namespace: public exception
    {
      public:
      invalid_namespace(const string &ns);
    };

    class invalid_query: public exception
    {
      public:
      invalid_query(const string &ns, const string &query);
    };

    enum class format_t
    {
      list,
      compact,
      value,
      json
    };
    const auto format_delimiters = std::map<format_t, string>{
      {format_t::list, " \n"},
      {format_t::compact, "="},
      {format_t::value, ""},
      {format_t::json, ""},
    };

    enum class si_unit_t
    {
      base,
      kilo,
      mega,
      giga,
      tera
    };
    const auto SI_PREC = 2;
    string human_format(const umax &n, const si_unit_t &from, const si_unit_t &to, const string &suf = "",
                        const umax &prec = SI_PREC);
    string human_format(const umax &n, const si_unit_t &to, const string &suf = "", const umax &prec = SI_PREC);
    string human_format(const umax &n, const string &suf = "", const umax &prec = SI_PREC);
    string human_format(const double &n, const si_unit_t &from, const si_unit_t &to, const string &suf = "",
                        const umax &prec = SI_PREC);
    string human_format(const double &n, const si_unit_t &to, const string &suf = "", const umax &prec = SI_PREC);
    string human_format(const double &n, const string &suf = "", const umax &prec = SI_PREC);
    void human_friendly_json(json &j);

    void stream(const format_t &format, vector<string> &pre, const json &j);
    void stream(const format_t &format, const std::map<string, vector<string>> &queries);
    void stream(const format_t &format, const vector<string> &namespaces);
    void stream(const format_t &format, const std::map<string, std::map<string, string>> &queries);

    argparse::ArgumentParser parse_args(int argc, char const *argv[]);
    std::map<string, vector<string>> select(const vector<string> &query);
    void list(argparse::ArgumentParser args);
    void get(argparse::ArgumentParser args);
    void run(argparse::ArgumentParser args);
    void main(int argc, char const *argv[]);
  } // namespace cli
} // namespace cspec

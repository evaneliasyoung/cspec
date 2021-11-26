/**
 *  @file      cli.h
 *  @brief     Command line interface for cspec.
 *
 *  @author    Evan Elias Young
 *  @date      2021-11-25
 *  @date      2021-11-25
 *  @copyright Copyright 2021 Evan Elias Young. All rights reserved.
 */

#pragma once

#include "../core.h"
#include "../lib/argparse/include/argparse/argparse.hpp"
#include "../utils/strcmp.hpp"

#include <iostream>
#include <map>

namespace cspec
{
  namespace cli
  {
    const std::map<string, vector<string>> queries = {
      {"cpu", {"all", "name", "amounts", "architecture", "clock", "endian", "vendor", "group"}},
      {"filesystem", {"all", "name", "sizes", "mount", "type"}},
      {"gpu", {"all", "name", "memory", "architecture"}},
      {"memory", {"all", "voltage", "form_factor", "size", "speed", "manufacturer", "model", "serial", "bank"}},
      {"system", {"all", "os", "kernel", "times"}}};
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

    argparse::ArgumentParser parse_args(int argc, char const *argv[]);
    void list(argparse::ArgumentParser args);
    void get(argparse::ArgumentParser args);
    void run(argparse::ArgumentParser args);
    void main(int argc, char const *argv[]);
  } // namespace cli
} // namespace cspec

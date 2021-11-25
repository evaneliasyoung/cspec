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

#include <iostream>

namespace cspec
{
  namespace cli
  {
    enum class format_t
    {
      list,
      compact,
      value,
      json
    };

    argparse::ArgumentParser parse_args(int argc, char const *argv[]);
    void stream(const json &j);
    void stream(const vector<string> &values);
    void stream(const format_t &format, const vector<string> &values);
    void stream(const format_t &format, const std::map<string, string> &keyval);
    void list(argparse::ArgumentParser args);
    void get(argparse::ArgumentParser args);
    void run(argparse::ArgumentParser args);
    void main(int argc, char const *argv[]);
  } // namespace cli
} // namespace cspec
